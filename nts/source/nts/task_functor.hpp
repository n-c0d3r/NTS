#pragma once

#include <nts/prerequisites.hpp>



namespace nts
{
    class F_coroutine;

    using F_task_functor_caller = void(F_coroutine&, u32 instance_index, void* data_p);

    template<typename F__>
    concept T_is_task_functor = requires(F__ f)
    {
        f(std::declval<F_coroutine&>(), u32(0));
    };
}