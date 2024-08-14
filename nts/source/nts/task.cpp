#include <nts/task.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{

    A_task::A_task(
        u32 parallel_count,
        F_task_counter* counter_p,
        E_task_priority priority,
        F_task_functor_caller* functor_caller_p,
        void* functor_p
    ) :
        parallel_count_(parallel_count),
        counter_p_(counter_p),
        priority_(priority),
        functor_caller_p_(functor_caller_p),
        functor_p_(functor_p)
    {
    }



    void A_task::execute(F_coroutine& coroutine, u32 begin_instance_index, u32 end_instance_index)
    {
        for(u32 instance_index = begin_instance_index; instance_index < end_instance_index; ++instance_index)
        {
            functor_caller_p_(coroutine, instance_index, functor_p_);
        }
        counter_p_->fetch_sub(end_instance_index - begin_instance_index);
    }

}