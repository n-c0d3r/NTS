#pragma once

#include <nts/prerequisites.hpp>

#include <nts/coroutine_desc.hpp>
#include <nts/coroutine_defs.hpp>
#include <nts/task_instance_range.hpp>


namespace nts
{

    class F_worker_thread;



    class NTS_API A_coroutine
    {
    private:
        F_coroutine_desc desc_;
        NCPP_ENABLE_IF_ASSERTION_ENABLED(
            TK<F_worker_thread> worker_thread_p_;
        );

        F_coroutine_resumer_caller* resumer_caller_p_ = 0;
        void* resumer_p_ = 0;

        F_task_instance_range task_instance_range_;

    public:
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE auto resumer_caller_p() const noexcept { return resumer_caller_p_; }
        NCPP_FORCE_INLINE auto resumer_p() const noexcept { return resumer_p_; }

        NCPP_FORCE_INLINE const auto& task_instance_range() const noexcept { return task_instance_range_; }



    protected:
        A_coroutine(F_coroutine_desc desc);

    public:
        virtual ~A_coroutine();



    protected:
        NCPP_ENABLE_IF_ASSERTION_ENABLED(
            void assert_worker_thread();
        );



    public:
        void bind_task_instance_range(const F_task_instance_range& task_instance_range);
        b8 resume(F_coroutine* from_coroutine_p);
        void pause();
        NCPP_FORCE_INLINE void yield()
        {
            resumer_caller_p_ = 0;
            resumer_p_ = 0;

            pause();
        }
        NCPP_FORCE_INLINE void yield(const auto& resumer)
        {
            using F_resumer = std::remove_const_t<std::remove_reference_t<decltype(resumer)>>;

            resumer_caller_p_ = [](void* resumer_p) -> b8
            {
                return (*((F_resumer*)resumer_p))();
            };
            resumer_p_ = (void*)&resumer;

            pause();
        }
    };

}
