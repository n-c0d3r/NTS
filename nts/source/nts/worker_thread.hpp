#pragma once

#include <nts/prerequisites.hpp>
#include <nts/frame_heap.hpp>
#include <nts/coroutine_defs.hpp>
#include <nts/task_context.hpp>
#include <nts/task_instance_range.hpp>
#include <nts/task_desc.hpp>
#include <nts/coroutine_pool.hpp>


namespace nts {

    class F_task_system;
    class F_worker_thread;



    namespace internal
    {
        extern thread_local F_worker_thread* current_worker_thread_raw_p;
        extern thread_local F_frame_param current_frame_param;
        extern thread_local F_coroutine* current_coroutine_p;
    }



    class NTS_API F_worker_thread
    {
    public:
        friend class F_task_system;

    public:
        using F_tick_functor = eastl::function<void(TKPA_valid<F_worker_thread>)>;



    private:
        u8 index_ = 0;
        u8 schedulable_index_ = 0;
        F_frame_memory_adapter* frame_memory_adapter_p_;
        TG_vector<F_frame_allocator> frame_allocators_;
        b8 is_schedulable_ = false;

        F_tick_functor tick_functor_;
        F_frame_param frame_param_ = 0;

        EA::Thread::Thread eathread_;

        F_coroutine* coroutine_p_ = 0;

        TG_array<
            TG_concurrent_ring_buffer<F_task_context>,
            u32(E_task_priority::COUNT)
        > task_context_ring_buffers_;

        TG_array<F_coroutine_pool, u32(E_coroutine_size::COUNT)> coroutine_pools_;

    public:
        NCPP_FORCE_INLINE u8 index() const noexcept { return index_; }
        NCPP_FORCE_INLINE b8 is_main() const noexcept { return index_ == 0; }
        NCPP_FORCE_INLINE u8 schedulable_index() const noexcept { return schedulable_index_; }
        NCPP_FORCE_INLINE auto& frame_memory_adapter() noexcept { return *frame_memory_adapter_p_; }
        NCPP_FORCE_INLINE auto& frame_allocators() noexcept { return frame_allocators_; }
        NCPP_FORCE_INLINE auto& frame_allocator(F_frame_param param) noexcept
        {
            return frame_allocators_[param];
        }
        NCPP_FORCE_INLINE b8 is_schedulable() const noexcept { return is_schedulable_; }

        NCPP_FORCE_INLINE const auto& tick_functor() const noexcept { return tick_functor_; }
        NCPP_FORCE_INLINE F_frame_param frame_param() const noexcept { return frame_param_; }

        NCPP_FORCE_INLINE auto& eathread() noexcept { return eathread_; }

        NCPP_FORCE_INLINE auto coroutine_p() noexcept { return coroutine_p_; }

        NCPP_FORCE_INLINE const auto& task_context_ring_buffers() noexcept { return task_context_ring_buffers_; }

        NCPP_FORCE_INLINE auto& coroutine_pools() noexcept { return coroutine_pools_; }



    public:
        F_worker_thread(u8 index, u8 schedulable_index);
        ~F_worker_thread();

    public:
        NCPP_OBJECT(F_worker_thread);



    private:
        void start_internal();
        void join_internal();

    private:
        void create_coroutine_internal();
        void destroy_coroutine_internal();

    private:
        void setup_thread_local_internal();

#ifdef NCPP_ENABLE_ASSERT
    private:
        void check_if_task_system_is_not_started();
#endif



    public:
        b8 tick();

    public:
        NCPP_FORCE_INLINE void install_tick(const F_tick_functor& tick_functor)
        {
#ifdef NCPP_ENABLE_ASSERT
            check_if_task_system_is_not_started();
#endif

            tick_functor_ = tick_functor;
        }
        NCPP_FORCE_INLINE void install_frame_param(F_frame_param frame_param)
        {
#ifdef NCPP_ENABLE_ASSERT
            check_if_task_system_is_not_started();
#endif

            frame_param_ = frame_param;
        }
        NCPP_FORCE_INLINE void install(const F_tick_functor& tick_functor, F_frame_param frame_param)
        {
            install_tick(tick_functor);
            install_frame_param(frame_param);
        }

    public:
        void schedule_task_context(F_task_context task_context, E_task_priority priority);
    };



    class H_worker_thread
    {
    public:
        static NCPP_FORCE_INLINE F_worker_thread* current_worker_thread_raw_p() { return internal::current_worker_thread_raw_p; }
        static NCPP_FORCE_INLINE TK_valid<F_worker_thread> current_worker_thread_p()
        {
            return TK_valid<F_worker_thread>::unsafe(current_worker_thread_raw_p());
        }
        static NCPP_FORCE_INLINE F_frame_param current_frame_param() { return internal::current_frame_param; }
        static NCPP_FORCE_INLINE F_coroutine* current_coroutine_p() { return internal::current_coroutine_p; }
    };

}
