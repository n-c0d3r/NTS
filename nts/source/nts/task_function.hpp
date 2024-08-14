#pragma once

#include <nts/prerequisites.hpp>



namespace nts
{

    class F_coroutine;

    using F_task_function = void(F_coroutine&, u32 instance_index, void* data_p);

}