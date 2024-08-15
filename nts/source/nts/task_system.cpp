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
                schedulable_thread_indices[non_ative_thread_index] = NCPP_U32_MAX;
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

            if(schedulable_thread_index != NCPP_U32_MAX)
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