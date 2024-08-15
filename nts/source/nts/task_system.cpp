#include <nts/task_system.hpp>
#include <nts/worker_thread.hpp>
#include <nts/task.hpp>



namespace nts {

    TK<F_task_system> F_task_system::instance_p_;



    namespace internal
    {
        u32 default_worker_thread_count()
        {
            return ceil(
                f32(std::thread::hardware_concurrency()) * 0.75f
            );
        }
        u32 parse_worker_thread_count(u32 n)
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
        ),
        cached_task_ring_buffer_(desc.cached_task_ring_buffer_capacity)
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

        TG_fixed_vector<u32, 64, false> active_thread_indices(desc_.worker_thread_count);

        // setup active_thread_indices
        {
            for(u32 i = 0; i < desc_.worker_thread_count; ++i)
            {
                active_thread_indices[i] = 0;
            }

            for(u32 non_ative_thread_index : desc_.non_active_thread_indices)
            {
                NCPP_ASSERT(non_ative_thread_index < desc_.worker_thread_count);
                active_thread_indices[non_ative_thread_index] = NCPP_U32_MAX;
            }

            u32 active_thread_index = 0;
            for(u32 i = 0; i < desc_.worker_thread_count; ++i)
            {
                if(active_thread_indices[i] == 0)
                {
                    active_thread_indices[i] = active_thread_index;
                    ++active_thread_index;
                }
            }
        }

        // create worker threads
        for(u32 i = 0; i < desc_.worker_thread_count; ++i)
        {
            u32 active_thread_index = active_thread_indices[i];

            worker_thread_p_vector_[i] = TU<F_worker_thread>()(
                i,
                active_thread_index
            );

            if(active_thread_index != NCPP_U32_MAX)
            {
                active_worker_thread_p_vector_.push_back(
                    NCPP_FOH_VALID(worker_thread_p_vector_[i])
                );
            }
        }
    }
    void F_task_system::destroy_worker_threads_internal()
    {
        active_worker_thread_p_vector_.clear();
        worker_thread_p_vector_.clear();
    }



    void F_task_system::start()
    {
        NCPP_ASSERT(!is_started_);

        is_started_ = true;

        schedule_cached_tasks_internal();
    }
    void F_task_system::join()
    {
        NCPP_ASSERT(!is_stopped_);

        is_stopped_ = true;
        destroy_worker_threads_internal();
    }



    void F_task_system::schedule_cached_tasks_internal()
    {
        F_task* task_p = 0;
        while(cached_task_ring_buffer_.try_pop(task_p))
        {
            schedule_task_internal(task_p);
        }
    }
    void F_task_system::cache_task_internal(
        F_task* task_p
    )
    {
        cached_task_ring_buffer_.push(task_p);
    }
    void F_task_system::schedule_task_internal(F_task* task_p)
    {
        // if task system is not started, cache the task to schedule it later
        if(!is_started_)
        {
            cache_task_internal(task_p);
        }
    }

}