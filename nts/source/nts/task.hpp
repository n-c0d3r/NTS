#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_functor.hpp>
#include <nts/task_desc.hpp>



namespace nts
{

    class NTS_API F_task
    {
    private:
        F_task_functor_caller* functor_caller_p_;
        void* functor_p_ = 0;
        F_task_desc desc_;

    public:
        NCPP_FORCE_INLINE F_task_functor_caller* functor_caller_p() const noexcept { return functor_caller_p_; }
        NCPP_FORCE_INLINE void* functor_p() const noexcept { return functor_p_; }
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }



    public:
        F_task(
            F_task_functor_caller* functor_caller_p,
            void* functor_p,
            F_task_desc desc
        );

    public:
        void execute(F_task_context context, u32 begin_instance_index = 0, u32 end_instance_index = 1);
    };

}