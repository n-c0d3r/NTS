#pragma once

#include <nts/prerequisites.hpp>

#ifdef EA_COMPILER_MSVC
#include <nts/msvc/coroutine.hpp>
#endif



namespace nts
{
#ifdef EA_COMPILER_MSVC
    using F_coroutine = F_msvc_coroutine;
#endif
}