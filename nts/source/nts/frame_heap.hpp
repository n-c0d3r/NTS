#pragma once

#include <nts/prerequisites.hpp>



namespace nts {

    class F_task_system;



    class NTS_API F_frame_heap : public TA_frame_heap<F_frame_heap>
    {
    private:
        static F_frame_heap* instance_p_;

    public:
        static NCPP_FORCE_INLINE F_frame_heap& instance() { return *instance_p_; }



    public:
        F_frame_heap(u32 adapter_count);
        ~F_frame_heap();
    };

    using F_frame_memory_adapter = F_frame_heap::F_adapter;
    using F_frame_allocator = F_frame_heap::F_allocator;



    class NTS_API F_reference_frame_allocator_config :
        public TF_reference_frame_allocator_config<F_frame_heap>
    {
    public:
        static F_frame_allocator* default_p();
    };

    using F_reference_frame_allocator = TF_reference_allocator<
        F_frame_allocator,
        F_reference_frame_allocator_config
    >;



    class NTS_API H_frame_heap
    {
    public:
        static void* allocate(sz size, sz alignment, sz alignment_offset, F_frame_param frame_param);
        static void* allocate(sz size, sz alignment, sz alignment_offset);
        static void deallocate(void* p, F_frame_param frame_param);
        static void deallocate(void* p);
    };

}