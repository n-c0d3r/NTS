#pragma once

#include <nts/prerequisites.hpp>



namespace nts
{
    class F_task;



    struct F_task_instance_range
    {
        F_task* task_p = 0;
        u32 begin_instance_index = 0;
        u32 end_instance_index = 0;

        NCPP_FORCE_INLINE operator b8 () const noexcept
        {
            return (task_p != 0);
        }
    };
}