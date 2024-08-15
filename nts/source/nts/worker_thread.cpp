#include <nts/worker_thread.hpp>
#include <nts/task_system.hpp>



namespace nts {

    namespace internal
    {
        thread_local F_worker_thread* current_worker_thread_raw_p;
        thread_local F_frame_param current_frame_param;
    }



    F_worker_thread::F_worker_thread(u32 index, u32 active_index) :
        index_(index),
        active_index_(active_index),
        is_active_(active_index != NCPP_U32_MAX),
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
    }
    void F_worker_thread::join_internal()
    {
    }

    void F_worker_thread::setup_thread_local_internal()
    {
        internal::current_worker_thread_raw_p = this;
        internal::current_frame_param = frame_param_;
    }

}