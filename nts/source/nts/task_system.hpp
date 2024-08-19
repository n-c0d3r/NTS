#pragma once

#include <nts/prerequisites.hpp>
#include <nts/frame_heap.hpp>
#include <nts/worker_thread.hpp>
#include <nts/task.hpp>
#include <nts/coroutine_defs.hpp>
#include <nts/coroutine_pool.hpp>


namespace nts {

    class F_task;
    class F_worker_thread;



    struct F_task_system_desc
    {
        u8 worker_thread_count = 0;
        TG_fixed_vector<u8, 64, false> non_schedulable_thread_indices = { 0 };

        u32 task_context_ring_buffer_capacity = NTS_DEFAULT_TASK_CONTEXT_RING_BUFFER_CAPACITY;
    };



    class NTS_API F_task_system
    {
    public:
        friend class F_worker_thread;



    private:
        static TK<F_task_system> instance_p_;

    public:
        static NCPP_FORCE_INLINE TK_valid<F_task_system> instance_p() { return (TKPA_valid<F_task_system>)instance_p_; }



    private:
        F_task_system_desc desc_;
        TG_vector<TU<F_worker_thread>> worker_thread_p_vector_;
        TG_vector<TK_valid<F_worker_thread>> schedulable_worker_thread_p_vector_;

        F_frame_heap frame_heap_;

        ab8 is_started_ = false;
        ab8 is_stopped_ = false;

        TG_array<F_coroutine_pool, u32(E_coroutine_size::COUNT)> coroutine_pools_;

        au8 ready_worker_thread_count_ = 1;

    public:
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE const auto& worker_thread_p_vector() const noexcept { return worker_thread_p_vector_; }
        NCPP_FORCE_INLINE const auto& schedulable_worker_thread_p_vector() const noexcept { return schedulable_worker_thread_p_vector_; }

        NCPP_FORCE_INLINE b8 is_started() const noexcept { return is_started_; }
        NCPP_FORCE_INLINE b8 is_stopped() const noexcept { return is_stopped_; }

        NCPP_FORCE_INLINE auto& coroutine_pools() noexcept { return coroutine_pools_; }



    public:
        F_task_system(const F_task_system_desc& desc);
        ~F_task_system();

    public:
        NCPP_OBJECT(F_task_system);



    private:
        void create_worker_threads_internal();
        void destroy_worker_threads_internal();

    private:
        void schedule_task_internal(F_task* task_p);



    public:
        void start();
        void join();

    public:
        F_task* schedule(
            auto&& functor,
            const F_task_desc& desc
        )
        {
            using F_functor = std::remove_const_t<std::remove_reference_t<decltype(functor)>>;
            NCPP_ASSERT(T_is_task_functor<F_functor>, "invalid functor type");

            // allocate memory for task pointer and functor
            void* allocated_memory_p_ = H_frame_heap::allocate(
                sizeof(F_task) + sizeof(F_functor),
                eastl::max(
                    NCPP_ALIGNOF(F_functor),
                    NCPP_ALIGNOF(F_task)
                ),
                0,
                desc.frame_param
            );

            // setup task and functor
            F_task* task_p = (F_task*)allocated_memory_p_;
            F_functor* functor_p = (F_functor*)(
                task_p + 1
            );

            new(task_p) F_task(
                [](u32 instance_index, void* data_p)
                {
                    (*((F_functor*)data_p))(
                        instance_index
                    );
                },
                [](void* data_p)
                {
                    ((F_functor*)data_p)->~F_functor();
                },
                functor_p,
                desc
            );
            new(functor_p) F_functor(
                NCPP_FORWARD(functor)
            );

            //
            desc.counter_p->add_fetch(desc.parallel_count);

            //
            schedule_task_internal(task_p);

            //
            return task_p;
        }

    };



    class H_task_system
    {
    public:
        static NCPP_FORCE_INLINE F_task* schedule(
            auto&& functor,
            const F_task_desc& desc
        )
        {
            return F_task_system::instance_p()->schedule(
                NCPP_FORWARD(functor),
                desc
            );
        }
    };

}
