#include <nts/task.hpp>



namespace nts
{

    A_task::A_task(
        u32 instance_count,
        F_task_counter* counter_p,
        E_task_priority priority,
        F_task_function* function_p,
        void* data_p
    ) :
        instance_count_(instance_count),
        counter_p_(counter_p),
        priority_(priority),
        function_p_(function_p),
        data_p_(data_p)
    {
    }

}