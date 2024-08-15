#pragma once

#include <nts/prerequisites.hpp>



namespace nts {

    enum class E_coroutine_size {

        SMALL, // 64 KiB
        MEDIUM, // 256 KiB
        LARGE, // 512 KiB

        COUNT,

        DEFAULT = MEDIUM

    };

    enum class E_coroutine_type {

        THREAD,
        TASK

    };

    struct F_coroutine_desc {

        E_coroutine_size size = E_coroutine_size::DEFAULT;
        E_coroutine_type type = E_coroutine_type::TASK;

    };

}