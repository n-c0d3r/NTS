#pragma once

#include <nts/prerequisites.hpp>

#include <nts/coroutine_defs.hpp>
#include <nts/coroutine_desc.hpp>


#define NTS_MAX_COROUTINE_POOL_CAPACITY 64



namespace nts
{
    class NTS_API F_coroutine_pool
    {
    private:
        E_coroutine_size coroutine_size_ = E_coroutine_size::DEFAULT;

        u32 capacity_ = NTS_MAX_COROUTINE_POOL_CAPACITY;
        F_coroutine_pool* parent_p_;

        TG_concurrent_ring_buffer<F_coroutine*> coroutine_ring_buffer_;

    public:
        NCPP_FORCE_INLINE E_coroutine_size coroutine_size() const noexcept { return coroutine_size_; }

        NCPP_FORCE_INLINE u32 capacity() const noexcept { return capacity_; }
        NCPP_FORCE_INLINE F_coroutine_pool* parent_p() const noexcept { return parent_p_; }

        NCPP_FORCE_INLINE const auto& coroutine_ring_buffer() const noexcept { return coroutine_ring_buffer_; }



    public:
        F_coroutine_pool() = default;
        F_coroutine_pool(E_coroutine_size coroutine_size, u32 capacity = NTS_MAX_COROUTINE_POOL_CAPACITY, F_coroutine_pool* parent_p = 0);
        ~F_coroutine_pool();

        F_coroutine_pool(const F_coroutine_pool& x) :
            coroutine_size_(x.coroutine_size_),
            capacity_(x.capacity_),
            parent_p_(x.parent_p_),
            coroutine_ring_buffer_(x.coroutine_ring_buffer_)
        {
        }
        F_coroutine_pool& operator = (const F_coroutine_pool& x)
        {
            coroutine_size_ = x.coroutine_size_;
            capacity_ = x.capacity_;
            parent_p_ = x.parent_p_;
            coroutine_ring_buffer_ = x.coroutine_ring_buffer_;

            return *this;
        }

        F_coroutine_pool(F_coroutine_pool&& x) :
            coroutine_size_(x.coroutine_size_),
            capacity_(x.capacity_),
            parent_p_(x.parent_p_),
            coroutine_ring_buffer_(std::move(x.coroutine_ring_buffer_))
        {
            x.parent_p_ = 0;
        }
        F_coroutine_pool& operator = (F_coroutine_pool&& x)
        {
            coroutine_size_ = x.coroutine_size_;
            capacity_ = x.capacity_;
            parent_p_ = x.parent_p_;
            coroutine_ring_buffer_ = std::move(x.coroutine_ring_buffer_);

            x.parent_p_ = 0;

            return *this;
        }



    private:
        F_coroutine* create_coroutine_internal();
        void destroy_coroutine_internal(F_coroutine* coroutine_p);



    public:
        F_coroutine* pop();
        void push(F_coroutine* coroutine_p);
    };

}
