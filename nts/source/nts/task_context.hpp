#pragma once

#include <nts/prerequisites.hpp>
#include <nts/coroutine.hpp>



namespace nts
{
    struct F_task_context
    {
    private:
        F_coroutine* coroutine_p_ = 0;

    public:
        NCPP_FORCE_INLINE F_coroutine* coroutine_p() const noexcept { return coroutine_p_; }


    public:
        NCPP_FORCE_INLINE F_task_context() noexcept = default;
        NCPP_FORCE_INLINE ~F_task_context() noexcept = default;

        NCPP_FORCE_INLINE F_task_context(F_coroutine* coroutine_p) noexcept :
            coroutine_p_(coroutine_p)
        {
        }

        NCPP_FORCE_INLINE F_task_context(const F_task_context& x) noexcept :
            coroutine_p_(x.coroutine_p_)
        {
        }
        NCPP_FORCE_INLINE F_task_context& operator = (const F_task_context& x) noexcept
        {
            coroutine_p_ = x.coroutine_p_;
            return *this;
        }



    public:
        void yield()
        {
            NCPP_ASSERT(coroutine_p_);
            coroutine_p_->yield();
        }
        void yield(const auto& resumer)
        {
            NCPP_ASSERT(coroutine_p_);
            coroutine_p_->yield(resumer);
        }
    };
}