/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/size_holder.hpp>
#include <actl/range/range_facade.hpp>
#include <actl/range/traits.hpp>
#include <actl/util/static_size.hpp>
#include <memory>

namespace ac {

template <class T, index N, class Alloc>
struct alloc_deleter
    : size_holder<N>
    , private Alloc
{
    explicit alloc_deleter(index size, Alloc alloc)
        : size_holder<N>{size}, Alloc{std::move(alloc)}
    {}

    void operator()(T* ptr) const
    {
        this->deallocate(ptr, this->size());
    }
};

template <class T, index N = dynamic_size, class Allocator = std::allocator<T>>
class heap_array
    : public range_facade<
          heap_array<T, N, Allocator>,
          dual_range_types<T*, const T*, index>>
{
public:
    using value_type = T;

    explicit heap_array() = default;

    explicit heap_array(std::initializer_list<T> il) : heap_array{span{il}} {}

    explicit heap_array(index size, const T& value)
        : heap_array{size, uninitialized}
    {
        std::uninitialized_fill_n(data(), size, value);
    }

    explicit heap_array(index size, uninitialized_t)
        : ptr_{Allocator{}.allocate(size), size}
    {}

    template <class R, enable_int_if<is_range_v<R>> = 0>
    explicit heap_array(R&& r)
        : heap_array{static_cast<index>(std::size(r)), uninitialized}
    {
        std::uninitialized_copy(std::begin(r), std::end(r), data());
    }

    T* data()
    {
        return ptr_->get();
    }
    const T* data() const
    {
        return ptr_->get();
    }

    index size() const
    {
        return ptr_.get_deleter().size();
    }

    void swap(heap_array& rhs)
    {
        std::swap(ptr_, rhs.ptr_);
    }

private:
    std::unique_ptr<T[], alloc_deleter<T, N, Allocator>> ptr_;
};

template <class T, index N, class A>
struct static_size<heap_array<T, N, A>> : index_constant<N>
{};

} // namespace ac
