#pragma once

#include <nts/prerequisites.hpp>

#include <nts/coroutine_desc.hpp>



namespace nts
{

    class NTS_API A_coroutine
    {
    private:
        F_coroutine_desc desc_;

    public:
        NCPP_FORCE_INLINE const auto& desc() const noexcept { return desc_; }



    protected:
        A_coroutine(F_coroutine_desc desc);

    public:
        ~A_coroutine();
    };

}