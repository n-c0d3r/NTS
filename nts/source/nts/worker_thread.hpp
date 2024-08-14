#pragma once

#include <nts/prerequisites.hpp>
#include <nts/frame_heap.hpp>



namespace nts {

    class F_task_system;
    class F_worker_thread;



    namespace internal
    {
        extern thread_local F_worker_thread* current_worker_thread_raw_p;
        extern thread_local F_frame_param current_frame_param;
    }



    class NTS_API F_worker_thread
    {
    public:
        friend class F_task_system;



    private:
        u32 index_ = 0;
        u32 active_index_ = 0;
        F_frame_memory_adapter* frame_memory_adapter_p_;
        TG_vector<F_frame_allocator> frame_allocators_;

    public:
        NCPP_FORCE_INLINE u32 index() const noexcept { return index_; }
        NCPP_FORCE_INLINE u32 active_index() const noexcept { return active_index_; }
        NCPP_FORCE_INLINE auto& frame_memory_adapter() noexcept { return *frame_memory_adapter_p_; }
        NCPP_FORCE_INLINE auto& frame_allocators() noexcept { return frame_allocators_; }
        NCPP_FORCE_INLINE auto& frame_allocator(F_frame_param param) noexcept
        {
            return frame_allocators_[param];
        }



    public:
        F_worker_thread(u32 index);
        ~F_worker_thread();

    public:
        NCPP_OBJECT(F_worker_thread);



    private:
        void join_internal();
    };



    class H_worker_thread
    {
    public:
        static NCPP_FORCE_INLINE F_worker_thread* current_worker_thread_raw_p() { return internal::current_worker_thread_raw_p; }
        static NCPP_FORCE_INLINE F_frame_param current_frame_param() { return internal::current_frame_param; }
    };

}