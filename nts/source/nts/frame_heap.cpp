#include <nts/frame_heap.hpp>
#include <nts/task_system.hpp>
#include <nts/worker_thread.hpp>



namespace nts
{

    F_frame_heap* F_frame_heap::instance_p_ = 0;



    F_frame_heap::F_frame_heap(u32 adapter_count) :
        TA_frame_heap<F_frame_heap>(
            NTS_FRAME_HEAP_PARAM_COUNT,
            adapter_count
        )
    {
        instance_p_ = this;
    }
    F_frame_heap::~F_frame_heap()
    {
    }



    F_frame_allocator* F_reference_frame_allocator_config::default_p()
    {
        return &(
            H_worker_thread::current_worker_thread_raw_p()->frame_allocator(
                H_worker_thread::current_frame_param()
            )
        );
    }


}