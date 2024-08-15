#pragma once

#include <nts/prerequisites.hpp>



namespace nts {

    class F_worker_thread;



    enum class E_coroutine_size {

        SMALL, // 64 KiB
        MEDIUM, // 256 KiB
        LARGE, // 512 KiB

        COUNT,

        UNKNOWN, // used for E_coroutine_type::THREAD

        DEFAULT = MEDIUM

    };

    enum class E_coroutine_type
    {
        THREAD,
        TASK
    };

    struct F_coroutine_desc {

        E_coroutine_size size = E_coroutine_size::DEFAULT;
        E_coroutine_type type = E_coroutine_type::TASK;

    };



    using F_coroutine_resumer_caller = b8(void* resumer_p);

}