#include <nts/msvc/coroutine.hpp>
#include <nts/worker_thread.hpp>
#include <nts/task.hpp>


namespace nts
{

    void msvc_coroutine_function(F_msvc_coroutine* coroutine_p)
    {
        while(true)
        {
            auto& task_instance_range = (F_task_instance_range&)(coroutine_p->task_instance_range());
            if(task_instance_range)
            {
                task_instance_range.task_p->execute(
                    { coroutine_p },
                    task_instance_range.begin_instance_index,
                    task_instance_range.end_instance_index
                );
                task_instance_range = {};

                // switch back to current worker thread
                coroutine_p->pause();
            }
        }
    }



    F_msvc_coroutine::F_msvc_coroutine(const F_coroutine_desc& desc) :
        A_coroutine(desc)
    {
        // convert thread to fiber
        if(desc.type == E_coroutine_type::THREAD)
        {
            NCPP_ASSERT(desc.size == E_coroutine_size::UNKNOWN) << "known coroutine size is not allowed for E_coroutine_type::THREAD";
            msvc_fiber_p_ = ConvertThreadToFiber(0);
        }
        // create fiber
        else
        {
            sz stack_size = 0;

            NCPP_ASSERT(desc.size != E_coroutine_size::UNKNOWN) << "unknown coroutine size is not allowed for E_coroutine_type::THREAD";
            switch (desc.size)
            {
            case E_coroutine_size::SMALL:
                stack_size = 64 << 10;
                break;
            case E_coroutine_size::MEDIUM:
                stack_size = 256 << 10;
                break;
            case E_coroutine_size::LARGE:
                stack_size = 512 << 10;
                break;
            }

            msvc_fiber_p_ = CreateFiber(
                stack_size,
                (LPFIBER_START_ROUTINE)&msvc_coroutine_function,
                this
            );
        }
    }
    F_msvc_coroutine::~F_msvc_coroutine()
    {
        NCPP_ASSERT(msvc_fiber_p_);

        // convert fiber to thread
        if(desc().type == E_coroutine_type::THREAD)
        {
            NCPP_ENABLE_IF_ASSERTION_ENABLED(
                assert_worker_thread();
            );
            ConvertFiberToThread();
        }
        // delete fiber
        else
        {
            DeleteFiber(msvc_fiber_p_);
        }
    }



    void F_msvc_coroutine::switch_to(F_msvc_coroutine* target_p)
    {
        NCPP_ASSERT(msvc_fiber_p_);
        NCPP_ASSERT(target_p);
        NCPP_ASSERT(target_p->msvc_fiber_p_);

        SwitchToFiber(target_p->msvc_fiber_p_);
    }

}
