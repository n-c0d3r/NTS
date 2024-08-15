#include <nts/worker_thread.hpp>
#include <nts/task_system.hpp>



namespace nts {

    namespace internal
    {
        thread_local F_worker_thread* current_worker_thread_raw_p;
        thread_local F_frame_param current_frame_param;
    }



    F_worker_thread::F_worker_thread(u8 index, u8 schedulable_index) :
        index_(index),
        schedulable_index_(schedulable_index),
        is_schedulable_(schedulable_index != NCPP_U32_MAX),
        frame_memory_adapter_p_(F_frame_heap::instance().adapter_p_vector()[index])
    {
        // create frame allocators
        for(u32 i = 0; i < F_frame_heap::instance().param_count(); ++i)
        {
            frame_allocators_.push_back(
                F_frame_allocator(frame_memory_adapter_p_, i)
            );
        }

        //
        if(index == 0)
            setup_thread_local_internal();

        //
        start_internal();
    }
    F_worker_thread::~F_worker_thread()
    {
        //
        join_internal();

        //
        frame_allocators_.clear();
    }



    void F_worker_thread::start_internal()
    {
        if(is_main())
            return;

        EA::Thread::ThreadParameters thread_parameters;
        thread_parameters.mnAffinityMask = 1 << (index_ - 1);

        eathread_ = EA::Thread::MakeThread(
            [this]()
            {
                while(
                    !(
                        F_task_system::instance_p()->is_stopped()
                    )
                )
                {
                    tick_internal();
                }
            },
            thread_parameters
        );
    }
    void F_worker_thread::join_internal()
    {
        if(is_main())
            return;

        eathread_.WaitForEnd();
    }

    void F_worker_thread::tick_internal()
    {
    }

    void F_worker_thread::setup_thread_local_internal()
    {
        internal::current_worker_thread_raw_p = this;
        internal::current_frame_param = frame_param_;
    }

}