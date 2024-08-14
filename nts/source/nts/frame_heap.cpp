#include <nts/frame_heap.hpp>
#include <nts/task_system.hpp>



namespace nts
{

    F_frame_heap::F_frame_heap() :
        TA_frame_heap<F_frame_heap>(
            NTS_FRAME_HEAP_PARAM_COUNT,
            F_task_system::instance_p()->desc().worker_thread_count
        )
    {
    }

}