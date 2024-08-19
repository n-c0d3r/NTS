#include <nts/task.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{
    F_task::F_task(
        F_task_functor_caller* functor_caller_p,
        F_task_functor_destructor_caller* functor_destructor_caller_p,
        void* functor_p,
        F_task_desc desc
    ) :
        functor_caller_p_(functor_caller_p),
        functor_destructor_caller_p_(functor_destructor_caller_p),
        functor_p_(functor_p),
        desc_(desc),
        local_counter_(desc.parallel_count)
    {
    }



    void F_task::execute(u32 begin_instance_index, u32 end_instance_index)
    {
        for(u32 instance_index = begin_instance_index; instance_index < end_instance_index; ++instance_index)
        {
            functor_caller_p_(instance_index, functor_p_);
        }

        u32 instance_count = end_instance_index - begin_instance_index;

        desc_.counter_p->fetch_sub(instance_count);

        // try release functor
        u32 prev_local_counter = local_counter_.fetch_sub(instance_count);
        if(prev_local_counter == instance_count)
        {
            functor_destructor_caller_p_(functor_p_);
        }
    }
}