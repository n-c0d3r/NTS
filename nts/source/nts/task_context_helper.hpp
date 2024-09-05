#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_context.hpp>
#include <nts/worker_thread.hpp>
#include <nts/task_system.hpp>
#include <nts/wait_for_counter.hpp>



namespace nts
{
    struct H_task_context
    {
    public:
        static NCPP_FORCE_INLINE F_task_context current()
        {
            return { H_worker_thread::current_coroutine_p() };
        }
        static NCPP_FORCE_INLINE void yield()
        {
            F_coroutine* coroutine_p = H_worker_thread::current_coroutine_p();
            NCPP_ASSERT(coroutine_p);
            coroutine_p->yield();
        }
        static NCPP_FORCE_INLINE void yield(auto&& resumer)
        {
            F_coroutine* coroutine_p = H_worker_thread::current_coroutine_p();
            NCPP_ASSERT(coroutine_p);
            coroutine_p->yield(resumer);
        }

    public:
        static NCPP_FORCE_INLINE void schedule_blocked(
            auto&& functor,
            F_task_desc&& desc = {}
        )
        {
            F_task_counter counter = 0;

            desc.counter_p = &counter;

            H_task_system::schedule(
                NCPP_FORWARD(functor),
                desc
            );

            yield(
                F_wait_for_counter(&counter)
            );
        }
        static NCPP_FORCE_INLINE void schedule_blocked(
            auto&& functor,
            const F_task_desc& desc = {}
        )
        {
            F_task_desc parsed_desc = desc;

            F_task_counter counter = 0;

            parsed_desc.counter_p = &counter;

            H_task_system::schedule(
                NCPP_FORWARD(functor),
                parsed_desc
            );

            yield(
                F_wait_for_counter(&counter)
            );
        }
        static NCPP_FORCE_INLINE void yield_or_block(auto&& resumer)
        {
            auto* current_worker_thread_raw_p = H_worker_thread::current_worker_thread_raw_p();
            if(current_worker_thread_raw_p->is_schedulable())
            {
                H_task_context::yield(NCPP_FORWARD(resumer));
            }
            else
            {
                while(!(resumer()));
            }
        }
    };
}