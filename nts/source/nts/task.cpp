#include <nts/task.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{

    F_task::F_task(
        F_task_functor_caller* functor_caller_p,
        void* functor_p,
        F_task_desc desc
    ) :
        functor_caller_p_(functor_caller_p),
        functor_p_(functor_p),
        desc_(desc)
    {
    }



    void F_task::execute(F_coroutine& coroutine, u32 begin_instance_index, u32 end_instance_index)
    {
        for(u32 instance_index = begin_instance_index; instance_index < end_instance_index; ++instance_index)
        {
            functor_caller_p_(coroutine, instance_index, functor_p_);
        }
        desc_.counter_p->fetch_sub(end_instance_index - begin_instance_index);
    }

}