#pragma once

#include <nts/prerequisites.hpp>

#include <nts/task_context.hpp>



namespace nts
{
    using F_task_functor_caller = void(u32 instance_index, void* data_p);
    using F_task_functor_destructor_caller = void(void* data_p);

    template<typename F__>
    concept T_is_task_functor = requires(F__ f)
    {
        f(u32(0));
    };
}