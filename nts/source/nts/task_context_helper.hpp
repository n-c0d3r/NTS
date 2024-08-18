#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_context.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{
    struct H_task_context
    {
    public:
        static F_task_context current()
        {
            return { H_worker_thread::current_coroutine_p() };
        }
        static void yield()
        {
            F_coroutine* coroutine_p = H_worker_thread::current_coroutine_p();
            NCPP_ASSERT(coroutine_p);
            coroutine_p->yield();
        }
        static void yield(auto&& resumer)
        {
            F_coroutine* coroutine_p = H_worker_thread::current_coroutine_p();
            NCPP_ASSERT(coroutine_p);
            coroutine_p->yield(resumer);
        }
    };
}