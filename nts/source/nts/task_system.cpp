#include <nts/task_system.hpp>



namespace nts {

    TK<F_task_system> F_task_system::instance_p_;



    F_task_system::F_task_system(const F_task_system_desc& desc) :
        desc_(desc)
    {
        instance_p_ = NCPP_KTHIS_UNSAFE();
    }

}