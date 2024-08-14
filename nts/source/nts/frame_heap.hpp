#pragma once

#include <nts/prerequisites.hpp>



namespace nts {

    class F_task_system;



    class NTS_API F_frame_heap : public TA_frame_heap<F_frame_heap>
    {
    public:
        F_frame_heap();
    };

}