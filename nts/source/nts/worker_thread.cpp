#include <nts/worker_thread.hpp>
#include <nts/task_system.hpp>



namespace nts {

    namespace internal
    {
        thread_local F_worker_thread* current_worker_thread_raw_p;
        thread_local F_frame_param current_frame_param;
    }



    F_worker_thread::F_worker_thread(u32 index) :
        index_(index),
        active_index_(
            F_task_system::instance_p()->desc().run_tasks_on_main_thread
            ? index
            : (index - 1)
        ),
        frame_memory_adapter_p_(F_frame_heap::instance().adapter_p_vector()[index])
    {
        // create frame allocators
        for(u32 i = 0; i < F_frame_heap::instance().param_count(); ++i)
        {
            frame_allocators_.push_back(
                F_frame_allocator(frame_memory_adapter_p_, i)
            );
        }
    }
    F_worker_thread::~F_worker_thread()
    {
    }



    void F_worker_thread::join_internal()
    {
    }

}