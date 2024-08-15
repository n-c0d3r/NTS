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
        b8 run_tasks_on_main_thread = true;
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

        b8 is_stopped_ = false;

    public:
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE const auto& worker_thread_p_vector() const noexcept { return worker_thread_p_vector_; }
        NCPP_FORCE_INLINE const auto& active_worker_thread_p_vector() const noexcept { return active_worker_thread_p_vector_; }

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
        void join();



    private:
        void schedule_task_internal(
            F_task* task,
            F_task_counter* counter_p,
            F_frame_param frame_param = 0,
            E_task_priority priority = E_task_priority::MEDIUM,
            u32 parallel_count = 1,
            u32 batch_size = NTS_DEFAULT_TASK_BATCH_SIZE
        );



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

            auto& frame_heap = F_frame_heap::instance();
            auto* worker_thread_raw_p = H_worker_thread::current_worker_thread_raw_p();
            auto& frame_allocator = worker_thread_raw_p->frame_allocator(
                frame_param
            );

            // allocate memory for task pointer and functor
            sz actual_allocated_size = sizeof(F_task*) + sizeof(F_functor);
            void* allocated_memory_p_ = frame_allocator.allocate(
                actual_allocated_size,
                NCPP_ALIGNOF(F_functor)
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
                parallel_count
            );

            //
            schedule_task_internal(
                task_p,
                counter_p,
                frame_param,
                priority,
                parallel_count,
                batch_size
            );

            //
            return task_p;
        }

    };

}
