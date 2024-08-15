#pragma once

#include <nts/prerequisites.hpp>



namespace nts
{
    class A_coroutine;

#ifdef EA_COMPILER_MSVC
    class F_msvc_coroutine;
#endif

#ifdef EA_COMPILER_MSVC
    using F_coroutine = F_msvc_coroutine;
#endif
}