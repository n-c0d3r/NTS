#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_functor.hpp>
#include <nts/task_counter.hpp>
#include <nts/task_priority.hpp>
#include <nts/coroutine_desc.hpp>



namespace nts
{

    struct F_task_desc
    {
        F_task_counter* counter_p = 0;
        F_frame_param frame_param = 0;
        E_task_priority priority = E_task_priority::DEFAULT;
        u32 parallel_count = 1;
        u32 batch_size = NTS_DEFAULT_TASK_BATCH_SIZE;
        E_coroutine_size coroutine_size = E_coroutine_size::DEFAULT;
    };

}