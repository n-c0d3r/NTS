#pragma once

#include <nts/prerequisites.hpp>

#include <nts/coroutine_base.hpp>



namespace nts
{

    class NTS_API F_msvc_coroutine : public A_coroutine
    {
    private:
        LPVOID msvc_fiber_p_ = 0;

    public:
        NCPP_FORCE_INLINE LPVOID msvc_fiber_p() const noexcept { return msvc_fiber_p_; }



    public:
        F_msvc_coroutine(const F_coroutine_desc& desc);
        ~F_msvc_coroutine();



    public:
        void switch_to(F_msvc_coroutine* target_p);
    };

}