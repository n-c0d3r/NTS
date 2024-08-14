#pragma once

#include <nts/prerequisites.hpp>



namespace nts {

    struct F_task_system_desc
    {
        u32 worker_thread_count = 0;
    };



    class NTS_API F_task_system
    {
    private:
        static TK<F_task_system> instance_p_;

    public:
        static NCPP_FORCE_INLINE TK_valid<F_task_system> instance_p() { return (TKPA_valid<F_task_system>)instance_p_; }



    private:
        F_task_system_desc desc_;

    public:
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }



    public:
        F_task_system(const F_task_system_desc& desc);
    };

}