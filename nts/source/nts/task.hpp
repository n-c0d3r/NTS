#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_functor.hpp>
#include <nts/task_counter.hpp>
#include <nts/task_priority.hpp>



namespace nts
{

    class NTS_API F_task
    {
    private:
        F_task_functor_caller* functor_caller_p_;
        void* functor_p_ = 0;
        F_task_counter* counter_p_ = 0;
        F_frame_param frame_param_;
        E_task_priority priority_;
        u32 parallel_count_ = 1;

    public:
        NCPP_FORCE_INLINE F_task_functor_caller* functor_caller_p() const noexcept { return functor_caller_p_; }
        NCPP_FORCE_INLINE void* functor_p() const noexcept { return functor_p_; }
        NCPP_FORCE_INLINE F_task_counter* counter_p() const noexcept { return counter_p_; }
        NCPP_FORCE_INLINE F_frame_param frame_param() const noexcept { return frame_param_; }
        NCPP_FORCE_INLINE E_task_priority priority() const noexcept { return priority_; }
        NCPP_FORCE_INLINE u32 parallel_count() const noexcept { return parallel_count_; }



    public:
        F_task(
            F_task_functor_caller* functor_caller_p,
            void* functor_p,
            F_task_counter* counter_p,
            F_frame_param frame_param,
            E_task_priority priority,
            u32 parallel_count
        );

    public:
        void execute(F_coroutine& coroutine, u32 begin_instance_index = 0, u32 end_instance_index = 1);
    };

}