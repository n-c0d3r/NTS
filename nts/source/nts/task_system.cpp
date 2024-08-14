#include <nts/task_system.hpp>
#include <nts/worker_thread.hpp>



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
        join();
    }



    void F_task_system::create_worker_threads_internal()
    {
        worker_thread_p_vector_.resize(desc_.worker_thread_count);

        for(u32 i = 0; i < desc_.worker_thread_count; ++i)
        {
            worker_thread_p_vector_[i] = TU<F_worker_thread>()(i);

            if(
                (i != 0)
                || desc_.run_tasks_on_main_thread
            )
            {
                active_worker_thread_p_vector_.push_back(
                    NCPP_FOH_VALID(worker_thread_p_vector_[i])
                );
            }
        }
    }
    void F_task_system::destroy_worker_threads_internal()
    {
        for(auto& active_worker_thread_p : active_worker_thread_p_vector_)
        {
            active_worker_thread_p->join_internal();
        }
        active_worker_thread_p_vector_.clear();
    }



    void F_task_system::start()
    {

    }
    void F_task_system::join()
    {
        if(is_stopped_)
            return;

        is_stopped_ = true;
        destroy_worker_threads_internal();
    }

}