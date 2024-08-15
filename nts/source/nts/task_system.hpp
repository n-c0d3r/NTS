#pragma once

#include <nts/prerequisites.hpp>
#include <nts/frame_heap.hpp>
#include <nts/worker_thread.hpp>
#include <nts/task.hpp>


namespace nts {

    class F_coroutine;
    class F_task;
    class F_worker_thread;



    struct F_task_system_desc
    {
        u32 worker_thread_count = 0;
        TG_fixed_vector<u32, 64, false> non_active_thread_indices = { 0 };

        u32 cached_task_ring_buffer_capacity = NTS_DEFAULT_CACHED_TASK_RING_BUFFER_CAPACITY;
    };



    class NTS_API F_task_system
    {
    private:
        static TK<F_task_system> instance_p_;

    public:
        static NCPP_FORCE_INLINE TK_valid<F_task_system> instance_p() { return (TKPA_valid<F_task_system>)instance_p_; }



    private:
        F_task_system_desc desc_;
        TG_vector<TU<F_worker_thread>> worker_thread_p_vector_;
        TG_vector<TK_valid<F_worker_thread>> active_worker_thread_p_vector_;

        F_frame_heap frame_heap_;

        TG_concurrent_ring_buffer<F_task*> cached_task_ring_buffer_;

        ab8 is_started_ = false;
        ab8 is_stopped_ = false;

    public:
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE const auto& worker_thread_p_vector() const noexcept { return worker_thread_p_vector_; }
        NCPP_FORCE_INLINE const auto& active_worker_thread_p_vector() const noexcept { return active_worker_thread_p_vector_; }

        NCPP_FORCE_INLINE const auto& cached_task_ring_buffer() const noexcept { return cached_task_ring_buffer_; }

        NCPP_FORCE_INLINE b8 is_started() const noexcept { return is_started_; }
        NCPP_FORCE_INLINE b8 is_stopped() const noexcept { return is_stopped_; }



    public:
        F_task_system(const F_task_system_desc& desc);
        ~F_task_system();

    public:
        NCPP_OBJECT(F_task_system);



    private:
        void create_worker_threads_internal();
        void destroy_worker_threads_internal();



    public:
        void start();
        void join();



    private:
        void schedule_cached_tasks_internal();
        void cache_task_internal(F_task* task_p);
        void schedule_task_internal(F_task* task_p);



    public:
        F_task* schedule(
            auto&& functor,
            F_task_counter* counter_p,
            F_frame_param frame_param = 0,
            E_task_priority priority = E_task_priority::MEDIUM,
            u32 parallel_count = 1,
            u32 batch_size = NTS_DEFAULT_TASK_BATCH_SIZE
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
                frame_param
            );

            // setup task and functor
            F_task* task_p = (F_task*)allocated_memory_p_;
            F_functor* functor_p = (F_functor*)(
                task_p + 1
            );

            *functor_p = NCPP_FORWARD(functor);

            new(task_p) F_task(
                [](F_coroutine& coroutine, u32 instance_index, void* data_p)
                {
                    (*((F_functor*)data_p))(
                        coroutine,
                        instance_index
                    );
                },
                functor_p,
                counter_p,
                frame_param,
                priority,
                parallel_count,
                batch_size
            );

            //
            counter_p->add_fetch(parallel_count);

            //
            schedule_task_internal(task_p);

            //
            return task_p;
        }

    };

}
