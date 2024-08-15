#include <nts/coroutine_base.hpp>
#include <nts/coroutine.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{

    A_coroutine::A_coroutine(F_coroutine_desc desc) :
        desc_(desc)
    {
        NCPP_ENABLE_IF_ASSERTION_ENABLED(
            if(desc.type == E_coroutine_type::THREAD)
                worker_thread_p_ = H_worker_thread::current_worker_thread_p().no_requirements();
        );
    }

    A_coroutine::~A_coroutine()
    {
        NCPP_ENABLE_IF_ASSERTION_ENABLED(
            assert_worker_thread();
        );
    }



    NCPP_ENABLE_IF_ASSERTION_ENABLED(
        void A_coroutine::assert_worker_thread()
        {
            if(desc_.type == E_coroutine_type::THREAD)
            {
                NCPP_ASSERT(worker_thread_p_);

                auto current_thread_id = EA::Thread::GetThreadId();

                const auto& eathread = worker_thread_p_->eathread();

                NCPP_ASSERT(eathread.GetId() == current_thread_id) << "coroutines have to be released on the threads that they were created from";
            }
        }
    );



    void A_coroutine::bind_task_instance_range(const F_task_instance_range& task_instance_range)
    {
        NCPP_ASSERT(!task_instance_range_) << "can't bind new task instance tange if the previous task instance range is not done";
        task_instance_range_ = task_instance_range;
    }
    b8 A_coroutine::resume(F_coroutine* from_coroutine_p)
    {
        //
        if(resumer_caller_p_)
        {
            if(
                !(resumer_caller_p_(resumer_p_))
            )
                return false;
        }

        //
        from_coroutine_p->switch_to((F_coroutine*)this);
        return !task_instance_range_;
    }
    void A_coroutine::pause()
    {
        ((F_coroutine*)this)->switch_to(
            H_worker_thread::current_worker_thread_raw_p()->coroutine_p()
        );
    }

}