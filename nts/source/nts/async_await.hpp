#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_context_helper.hpp>
#include <nts/frame_heap.hpp>



namespace nts
{
    namespace internal
    {
        struct F_await_caller
        {
        };
        struct F_await_blockable_caller
        {
        };
    }



    template<typename F_return__>
    struct TF_promise
    {
    public:
        using F_return = F_return__;



    private:
        F_task_counter* counter_p_ = 0;
        F_return* return_value_p_ = 0;

    public:
        NCPP_FORCE_INLINE F_task_counter* counter_p() const noexcept { return counter_p_; }
        NCPP_FORCE_INLINE F_return* return_value_p() const noexcept { return return_value_p_; }
        NCPP_FORCE_INLINE F_task_counter& counter() const noexcept { return *counter_p_; }
        NCPP_FORCE_INLINE F_return& return_value() const noexcept { return *return_value_p_; }



    public:
        NCPP_FORCE_INLINE TF_promise() noexcept = default;
        NCPP_FORCE_INLINE TF_promise(
            auto&& functor,
            const F_task_desc& desc = {}
        ) :
            counter_p_(
                H_frame_heap::T_create<F_task_counter>(0)
            ),
            return_value_p_(
                (F_return*)H_frame_heap::allocate(
                    sizeof(F_return),
                    NCPP_ALIGNOF(F_return),
                    0
                )
            )
        {
            F_task_desc parsed_desc = desc;

            parsed_desc.counter_p = counter_p_;

            F_return* return_value_p = return_value_p_;

            H_task_system::schedule(
                [f = NCPP_FORWARD(functor), return_value_p](u32 instance_index)
                {
                    *return_value_p = f(instance_index);
                },
                parsed_desc
            );
        }
        NCPP_FORCE_INLINE TF_promise(
            auto&& functor,
            F_task_desc&& desc
        ) :
            counter_p_(
                H_frame_heap::T_create<F_task_counter>(0)
            ),
            return_value_p_(
                (F_return*)H_frame_heap::allocate(
                    sizeof(F_return),
                    NCPP_ALIGNOF(F_return),
                    0
                )
            )
        {
            desc.counter_p = counter_p_;

            F_return* return_value_p = return_value_p_;

            H_task_system::schedule(
                [f = NCPP_FORWARD(functor), return_value_p](u32 instance_index)
                {
                    *return_value_p = f(instance_index);
                },
                std::move(desc)
            );
        }

        NCPP_FORCE_INLINE TF_promise(const TF_promise& x) noexcept :
            counter_p_(x.counter_p_),
            return_value_p_(x.return_value_p_)
        {
        }
        NCPP_FORCE_INLINE TF_promise& operator = (const TF_promise& x) noexcept
        {
            counter_p_ = x.counter_p_;
            return_value_p_ = x.return_value_p_;

            return *this;
        }



    public:
        NCPP_FORCE_INLINE b8 is_valid() const noexcept { return (counter_p_ != 0) && (return_value_p_ != 0); }
        NCPP_FORCE_INLINE b8 is_done() const noexcept
        {
            return (
                0 == (counter_p_->load(eastl::memory_order_acquire))
            );
        }
    };

    template<>
    struct TF_promise<void>
    {
    public:
        using F_return = void;



    private:
        F_task_counter* counter_p_ = 0;

    public:
        NCPP_FORCE_INLINE F_task_counter* counter_p() const noexcept { return counter_p_; }
        NCPP_FORCE_INLINE F_task_counter& counter() const noexcept { return *counter_p_; }



    public:
        NCPP_FORCE_INLINE TF_promise() noexcept = default;
        NCPP_FORCE_INLINE TF_promise(
            auto&& functor,
            const F_task_desc& desc = {}
        ) :
            counter_p_(
                H_frame_heap::T_create<F_task_counter>(0)
            )
        {
            F_task_desc parsed_desc = desc;

            parsed_desc.counter_p = counter_p_;

            H_task_system::schedule(
                NCPP_FORWARD(functor),
                parsed_desc
            );
        }
        NCPP_FORCE_INLINE TF_promise(
            auto&& functor,
            F_task_desc&& desc
        ) :
            counter_p_(
                H_frame_heap::T_create<F_task_counter>(0)
            )
        {
            desc.counter_p = counter_p_;

            H_task_system::schedule(
                NCPP_FORWARD(functor),
                std::move(desc)
            );
        }

        NCPP_FORCE_INLINE TF_promise(const TF_promise& x) noexcept :
            counter_p_(x.counter_p_)
        {
        }
        NCPP_FORCE_INLINE TF_promise& operator = (const TF_promise& x) noexcept
        {
            counter_p_ = x.counter_p_;

            return *this;
        }



    public:
        NCPP_FORCE_INLINE b8 is_valid() const noexcept { return (counter_p_ != 0); }
        NCPP_FORCE_INLINE b8 is_done() const noexcept
        {
            return (
                0 == (counter_p_->load(eastl::memory_order_acquire))
            );
        }
    };
}



NCPP_FORCE_INLINE auto operator << (const nts::internal::F_await_caller&, const auto& promise)
-> typename std::remove_const_t<std::remove_reference_t<decltype(promise)>>::F_return
{
    using namespace nts;

    if(promise.counter())
        H_task_context::yield(
            F_wait_for_counter(promise.counter_p())
        );

    return promise.return_value();
}
NCPP_FORCE_INLINE void operator << (const nts::internal::F_await_caller&, const nts::TF_promise<void>& promise)
{
    using namespace nts;

    if(promise.counter())
        H_task_context::yield(
            F_wait_for_counter(promise.counter_p())
        );
}

NCPP_FORCE_INLINE auto operator << (const nts::internal::F_await_blockable_caller&, const auto& promise)
-> typename std::remove_const_t<std::remove_reference_t<decltype(promise)>>::F_return
{
    using namespace nts;

    if(promise.counter())
        H_task_context::yield_or_block(
            F_wait_for_counter(promise.counter_p())
        );

    return promise.return_value();
}
NCPP_FORCE_INLINE void operator << (const nts::internal::F_await_blockable_caller&, const nts::TF_promise<void>& promise)
{
    using namespace nts;

    if(promise.counter())
        H_task_context::yield_or_block(
            F_wait_for_counter(promise.counter_p())
        );
}



NCPP_FORCE_INLINE auto NTS_ASYNC(auto&& functor, const nts::F_task_desc& desc = {})
{
    return nts::TF_promise<decltype(functor(ncpp::u32(0)))> {
        NCPP_FORWARD(functor),
        desc
    };
}
NCPP_FORCE_INLINE auto NTS_ASYNC(auto&& functor, nts::F_task_desc&& desc)
{
    return nts::TF_promise<decltype(functor(ncpp::u32(0)))> {
        NCPP_FORWARD(functor),
        std::move(desc)
    };
}

#define NTS_AWAIT nts::internal::F_await_caller() <<

#define NTS_AWAIT_BLOCKABLE nts::internal::F_await_blockable_caller() <<