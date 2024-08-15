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

    public:
        using F_tick_functor = eastl::function<void(TKPA_valid<F_worker_thread>)>;



    private:
        u32 index_ = 0;
        u32 active_index_ = 0;
        F_frame_memory_adapter* frame_memory_adapter_p_;
        TG_vector<F_frame_allocator> frame_allocators_;
        b8 is_active_ = false;

        F_tick_functor tick_functor_;
        F_frame_param frame_param_ = 0;

    public:
        NCPP_FORCE_INLINE u32 index() const noexcept { return index_; }
        NCPP_FORCE_INLINE u32 active_index() const noexcept { return active_index_; }
        NCPP_FORCE_INLINE auto& frame_memory_adapter() noexcept { return *frame_memory_adapter_p_; }
        NCPP_FORCE_INLINE auto& frame_allocators() noexcept { return frame_allocators_; }
        NCPP_FORCE_INLINE auto& frame_allocator(F_frame_param param) noexcept
        {
            return frame_allocators_[param];
        }
        NCPP_FORCE_INLINE b8 is_active() const noexcept { return is_active_; }

        NCPP_FORCE_INLINE const auto& tick_functor() const noexcept { return tick_functor_; }
        NCPP_FORCE_INLINE F_frame_param frame_param() const noexcept { return frame_param_; }



    public:
        F_worker_thread(u32 index, u32 active_index);
        ~F_worker_thread();

    public:
        NCPP_OBJECT(F_worker_thread);



    private:
        void start_internal();
        void join_internal();

    private:
        void setup_thread_local_internal();



    public:
        NCPP_FORCE_INLINE void install_tick(const F_tick_functor& tick_functor)
        {
            tick_functor_ = tick_functor;
        }
        NCPP_FORCE_INLINE void install_frame_param(F_frame_param frame_param)
        {
            frame_param_ = frame_param;
        }
        NCPP_FORCE_INLINE void install(const F_tick_functor& tick_functor, F_frame_param frame_param)
        {
            install_tick(tick_functor);
            install_frame_param(frame_param);
        }
    };



    class H_worker_thread
    {
    public:
        static NCPP_FORCE_INLINE F_worker_thread* current_worker_thread_raw_p() { return internal::current_worker_thread_raw_p; }
        static NCPP_FORCE_INLINE F_frame_param current_frame_param() { return internal::current_frame_param; }
    };

}