#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_counter.hpp>



namespace nts
{
    struct F_wait_for_counter
    {
    private:
        F_task_counter* counter_p_ = 0;

    public:
        NCPP_FORCE_INLINE auto counter_p() const noexcept { return counter_p_; }



    public:
        NCPP_FORCE_INLINE F_wait_for_counter(F_task_counter* counter_p) noexcept :
            counter_p_(counter_p)
        {
        }



    public:
        NCPP_FORCE_INLINE b8 operator () () const noexcept
        {
            return (*counter_p_ == 0);
        }
    };
}