#pragma once

#include <nts/prerequisites.hpp>

#include <nts/coroutine_base.hpp>



namespace nts
{

    class NTS_API F_msvc_coroutine : public A_coroutine
    {
    public:
        F_msvc_coroutine(const F_coroutine_desc& desc);
        ~F_msvc_coroutine();
    };

}