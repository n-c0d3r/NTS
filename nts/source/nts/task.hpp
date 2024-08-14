#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_function.hpp>
#include <nts/task_counter.hpp>
#include <nts/task_priority.hpp>



namespace nts
{

    class NTS_API A_task
    {
    private:
        u32 instance_count_ = 1;
        F_task_counter* counter_p_ = 0;
        E_task_priority priority_;
        F_task_function* function_p_;
        void* data_p_ = 0;

    public:
        NCPP_FORCE_INLINE u32 instance_count() const noexcept { return instance_count_; }
        NCPP_FORCE_INLINE F_task_counter* counter_p() const noexcept { return counter_p_; }
        NCPP_FORCE_INLINE E_task_priority priority() const noexcept { return priority_; }
        NCPP_FORCE_INLINE F_task_function* function_p() const noexcept { return function_p_; }
        NCPP_FORCE_INLINE void* data_p() const noexcept { return data_p_; }



    public:
        A_task(
            u32 instance_count,
            F_task_counter* counter_p,
            E_task_priority priority,
            F_task_function* function_p,
            void* data_p
        );

    public:
        NCPP_FORCE_INLINE void execute(F_coroutine& coroutine, u32 begin_instance_index = 0, u32 end_instance_index = 1)
        {
            for(u32 instance_index = begin_instance_index; instance_index < end_instance_index; ++instance_index)
            {
                function_p_(coroutine, instance_index, data_p_);
            }
            counter_p_->fetch_sub(end_instance_index - begin_instance_index);
        }
    };

    // template<typename F_data__>

}