#include <nts/task.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{

    F_task::F_task(
        F_task_functor_caller* functor_caller_p,
        void* functor_p,
        F_task_counter* counter_p,
        F_frame_param frame_param,
        E_task_priority priority,
        u32 parallel_count,
        u32 batch_size
    ) :
        functor_caller_p_(functor_caller_p),
        functor_p_(functor_p),
        counter_p_(counter_p),
        frame_param_(frame_param),
        priority_(priority),
        parallel_count_(parallel_count),
        batch_size_(batch_size)
    {
    }



    void F_task::execute(F_coroutine& coroutine, u32 begin_instance_index, u32 end_instance_index)
    {
        for(u32 instance_index = begin_instance_index; instance_index < end_instance_index; ++instance_index)
        {
            functor_caller_p_(coroutine, instance_index, functor_p_);
        }
        counter_p_->fetch_sub(end_instance_index - begin_instance_index);
    }

}