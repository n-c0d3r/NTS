#include <nts/coroutine_pool.hpp>
#include <nts/coroutine.hpp>



namespace nts
{

    F_coroutine_pool::F_coroutine_pool(E_coroutine_size coroutine_size, u32 capacity, F_coroutine_pool* parent_p) :
        coroutine_size_(coroutine_size),
        capacity_(capacity),
        parent_p_(parent_p),
        coroutine_ring_buffer_(
            capacity
            * 2
        )
    {
        NCPP_ASSERT(capacity <= NTS_MAX_COROUTINE_POOL_CAPACITY);
    }
    F_coroutine_pool::~F_coroutine_pool()
    {
    }



    F_coroutine* F_coroutine_pool::create_coroutine_internal()
    {
        return new F_coroutine({
            .size = coroutine_size_
        });
    }
    void F_coroutine_pool::destroy_coroutine_internal(F_coroutine* coroutine_p)
    {
        delete coroutine_p;
    }



    F_coroutine* F_coroutine_pool::pop()
    {
        F_coroutine* coroutine_p;
        if(
            !coroutine_ring_buffer_.try_pop(coroutine_p)
        )
            coroutine_p = create_coroutine_internal();

        return coroutine_p;
    }
    void F_coroutine_pool::push(F_coroutine* coroutine_p)
    {
        u32 prev_size = coroutine_ring_buffer_.size();

        if(prev_size >= capacity_)
        {
            if(parent_p_)
                parent_p_->push(coroutine_p);
            else
                destroy_coroutine_internal(coroutine_p);
        }
        else
        {
            coroutine_ring_buffer_.push(coroutine_p);
        }
    }
}