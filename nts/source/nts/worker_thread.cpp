#include <nts/worker_thread.hpp>
#include <nts/task_system.hpp>
#include <nts/coroutine.hpp>



namespace nts {

    namespace internal
    {
        thread_local F_worker_thread* current_worker_thread_raw_p;
        thread_local F_frame_param current_frame_param;
    }



    F_worker_thread::F_worker_thread(u8 index, u8 schedulable_index) :
        index_(index),
        schedulable_index_(schedulable_index),
        is_schedulable_(schedulable_index != NCPP_U8_MAX),
        frame_memory_adapter_p_(F_frame_heap::instance().adapter_p_vector()[index])
    {
        // setup task context ring buffers
        u32 task_context_ring_buffer_capacity = F_task_system::instance_p()->desc().task_context_ring_buffer_capacity;
        for(u32 i = 0; i < u32(E_task_priority::COUNT); ++i)
        {
            task_context_ring_buffers_[i] = TG_concurrent_ring_buffer<F_task_context>(
                task_context_ring_buffer_capacity
            );
        }

        // setup coroutine pools
        auto& parent_coroutine_pools = F_task_system::instance_p()->coroutine_pools();
        for(u32 i = 0; i < u32(E_coroutine_size::COUNT); ++i)
        {
            coroutine_pools_[i] = F_coroutine_pool(
                E_coroutine_size(i),
                NTS_MAX_COROUTINE_POOL_CAPACITY,
                &parent_coroutine_pools[i]
            );
        }

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
        thread_parameters.mnAffinityMask = u64(1) << u64(index_ - 1);

        eathread_ = EA::Thread::MakeThread(
            [this]()
            {
                //
                setup_thread_local_internal();

                //
                create_coroutine_internal();

                // wait for the task scheduler started
                while(
                    !(F_task_system::instance_p()->is_started())
                );

                // main loop
                while(tick());

                //
                destroy_coroutine_internal();
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

    void F_worker_thread::create_coroutine_internal()
    {
        NCPP_ASSERT(!coroutine_p_);
        coroutine_p_ = new F_coroutine({
            .size = E_coroutine_size::UNKNOWN,
            .type = E_coroutine_type::THREAD
        });
    }
    void F_worker_thread::destroy_coroutine_internal()
    {
        NCPP_ASSERT(coroutine_p_);
        delete coroutine_p_;
    }

    void F_worker_thread::setup_thread_local_internal()
    {
        internal::current_worker_thread_raw_p = this;
        internal::current_frame_param = frame_param_;
    }



    b8 F_worker_thread::tick()
    {
        // if this worker thread is not schedulable, just call tick functor
        if(!is_schedulable_)
        {
            tick_functor_(NCPP_KTHIS());
            return !(F_task_system::instance_p()->is_stopped());
        }

        // if there is some task context ring buffers that are not empty, do not stop
        for(const auto& ring_buffer : task_context_ring_buffers_)
            if(ring_buffer.size())
                goto has_to_do_works;

        has_to_do_works:

        // the pop count for each task priority
        static u32 pop_counts[u32(E_task_priority::COUNT)] = {
            1, // LOW
            2, // MEDIUM
            4 // HIGH
        };

        // try pop and execute task contexts
        for(u32 i = 0; i < u32(E_task_priority::COUNT); ++i)
        {
            auto& task_context_ring_buffer = task_context_ring_buffers_[i];

            u32 pop_count = eastl::min<u32>(pop_counts[i], task_context_ring_buffer.size());

            for(u32 j = 0; j < pop_count; ++j)
            {
                F_task_context task_context;
                if(task_context_ring_buffer.try_pop(task_context))
                {
                    F_coroutine* coroutine_p = task_context.coroutine_p();

                    F_frame_param prev_frame_param = internal::current_frame_param;
                    internal::current_frame_param = coroutine_p->frame_param();

                    if(coroutine_p->resume(coroutine_p_))
                    {
                        internal::current_frame_param = prev_frame_param;

                        E_coroutine_size coroutine_size = coroutine_p->desc().size;

                        coroutine_pools_[u32(coroutine_size)].push(coroutine_p);
                    }
                    else
                    {
                        internal::current_frame_param = prev_frame_param;

                        task_context_ring_buffer.push(task_context);
                    }
                }
                else break;
            }
        }

        return !(F_task_system::instance_p()->is_stopped());
    }

    void F_worker_thread::schedule_task_context(F_task_context task_context, E_task_priority priority)
    {
        task_context_ring_buffers_[u32(priority)].push(task_context);
    }

}