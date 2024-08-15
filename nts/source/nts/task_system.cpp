#include <nts/task_system.hpp>
#include <nts/worker_thread.hpp>
#include <nts/task.hpp>



namespace nts {

    TK<F_task_system> F_task_system::instance_p_;



    namespace internal
    {
        u8 default_worker_thread_count()
        {
            return ceil(
                f32(std::thread::hardware_concurrency()) * 0.75f
            );
        }
        u8 parse_worker_thread_count(u8 n)
        {
            if(n == 0)
                return default_worker_thread_count();

            return n;
        }
    }



    F_task_system::F_task_system(const F_task_system_desc& desc) :
        desc_(desc),
        frame_heap_(
            internal::parse_worker_thread_count(desc.worker_thread_count)
        )
    {
        instance_p_ = NCPP_KTHIS_UNSAFE();

        //
        desc_.worker_thread_count = internal::parse_worker_thread_count(desc.worker_thread_count);

        // setup coroutine pools
        for(u32 i = 0; i < u32(E_coroutine_size::COUNT); ++i)
        {
            coroutine_pools_[i] = F_coroutine_pool(
                E_coroutine_size(i)
            );
        }

        //
        create_worker_threads_internal();
    }
    F_task_system::~F_task_system()
    {
        if(!is_stopped_)
            join();
    }



    void F_task_system::create_worker_threads_internal()
    {
        worker_thread_p_vector_.resize(desc_.worker_thread_count);

        TG_fixed_vector<u8, 64, false> schedulable_thread_indices(desc_.worker_thread_count);

        // setup schedulable_thread_indices
        {
            for(u8 i = 0; i < desc_.worker_thread_count; ++i)
            {
                schedulable_thread_indices[i] = 0;
            }

            for(u8 non_ative_thread_index : desc_.non_schedulable_thread_indices)
            {
                NCPP_ASSERT(non_ative_thread_index < desc_.worker_thread_count);
                schedulable_thread_indices[non_ative_thread_index] = NCPP_U8_MAX;
            }

            u8 schedulable_thread_index = 0;
            for(u8 i = 0; i < desc_.worker_thread_count; ++i)
            {
                if(schedulable_thread_indices[i] == 0)
                {
                    schedulable_thread_indices[i] = schedulable_thread_index;
                    ++schedulable_thread_index;
                }
            }
        }

        // create worker threads
        for(u8 i = 0; i < desc_.worker_thread_count; ++i)
        {
            u8 schedulable_thread_index = schedulable_thread_indices[i];

            worker_thread_p_vector_[i] = TU<F_worker_thread>()(
                i,
                schedulable_thread_index
            );

            if(schedulable_thread_index != NCPP_U8_MAX)
            {
                schedulable_worker_thread_p_vector_.push_back(
                    NCPP_FOH_VALID(worker_thread_p_vector_[i])
                );
            }
        }
    }
    void F_task_system::destroy_worker_threads_internal()
    {
        schedulable_worker_thread_p_vector_.clear();
        worker_thread_p_vector_.clear();
    }

    void F_task_system::schedule_task_internal(F_task* task_p)
    {
        E_task_priority priority = task_p->desc().priority;
        u32 parallel_count = task_p->desc().parallel_count;
        u32 batch_size = task_p->desc().batch_size;
        E_coroutine_size coroutine_size = task_p->desc().coroutine_size;

        NCPP_ASSERT(parallel_count);
        NCPP_ASSERT(batch_size);

        u32 batch_count = ceil(
            f32(parallel_count)
            / f32(batch_size)
        );

        auto* current_worker_thread_raw_p = H_worker_thread::current_worker_thread_raw_p();

        u32 schedulable_worker_thread_count = schedulable_worker_thread_p_vector_.size();
        u32 schedulable_worker_thread_index = current_worker_thread_raw_p->schedulable_index();

        auto& coroutine_pool = current_worker_thread_raw_p->coroutine_pools()[
            u32(coroutine_size)
        ];

        u32 begin_task_instance_index = 0;
        u32 remain_instance_count = parallel_count;

        for(u32 i = 0; i < batch_count; ++i)
        {
            schedulable_worker_thread_index = (schedulable_worker_thread_index + 1) % schedulable_worker_thread_count;

            // setup coroutine
            F_coroutine* coroutine_p = coroutine_pool.pop();

            F_task_instance_range task_instance_range = {
                .task_p = task_p,
                .begin_instance_index = begin_task_instance_index
            };

            u32 instance_count = eastl::min(remain_instance_count, batch_size);
            remain_instance_count -= instance_count;

            begin_task_instance_index += instance_count;

            task_instance_range.end_instance_index = begin_task_instance_index;

            coroutine_p->bind_task_instance_range(task_instance_range);

            // schedule task context on another worker thread
            F_task_context task_context(coroutine_p);
            const auto& worker_thread_p = schedulable_worker_thread_p_vector_[schedulable_worker_thread_index];
            worker_thread_p->schedule_task_context(task_context, priority);
        }
    }



    void F_task_system::start()
    {
        NCPP_ASSERT(!is_started_);

        is_started_ = true;
    }
    void F_task_system::join()
    {
        NCPP_ASSERT(!is_stopped_);

        is_stopped_ = true;
        destroy_worker_threads_internal();
    }

}