#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_functor.hpp>
#include <nts/task_counter.hpp>
#include <nts/task_priority.hpp>



namespace nts
{

    class NTS_API A_task
    {
    private:
        u32 parallel_count_ = 1;
        F_task_counter* counter_p_ = 0;
        E_task_priority priority_;
        F_task_functor_caller* functor_caller_p_;
        void* functor_p_ = 0;

    public:
        NCPP_FORCE_INLINE u32 parallel_count() const noexcept { return parallel_count_; }
        NCPP_FORCE_INLINE F_task_counter* counter_p() const noexcept { return counter_p_; }
        NCPP_FORCE_INLINE E_task_priority priority() const noexcept { return priority_; }
        NCPP_FORCE_INLINE F_task_functor_caller* functor_caller_p() const noexcept { return functor_caller_p_; }
        NCPP_FORCE_INLINE void* functor_p() const noexcept { return functor_p_; }



    public:
        A_task(
            u32 parallel_count,
            F_task_counter* counter_p,
            E_task_priority priority,
            F_task_functor_caller* functor_caller_p,
            void* functor_p
        );

    public:
        void execute(F_coroutine& coroutine, u32 begin_instance_index = 0, u32 end_instance_index = 1);
    };

    // template<typename F_data__>

}