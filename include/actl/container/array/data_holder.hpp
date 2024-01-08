/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/std/array.hpp>
#include <memory>

namespace ac {

struct uninitialized_t
{};
inline constexpr uninitialized_t uninitialized;

template <class Data>
class data_holder;

template <class T, size_t Size>
class data_holder<std::array<T, Size>>
{
public:
    using value_type = T;

    explicit data_holder(int size) : data_holder{size, ValueType{}} {}

    template <class InRange>
    explicit data_holder(index size, InRange in)
        : data_holder{size, uninitialized}
    {
        ACTL_ASSERT(size == static_cast<index>(std::size(in)));
        std::unitialized_copy_n(std::begin(in), size, data());
    }

    explicit data_holder(index size, const T& value)
        : data_holder{size, uninitialized}
    {
        std::unitialized_fill_n(data(), size, value);
    }

    explicit data_holder(index size, uninitialized_t)
    {
        ACTL_ASSERT(size == index{Size});
    }

    T* data()
    {
        return data_.data();
    }
    const T* data() const
    {
        return data_.data();
    }

    void swap(data_holder& rhs)
    {
        std::swap(data_, rhs.data_);
    }

private:
    std::array<T, Size> data_;
};

template <class Alloc>
struct deleter
{
    void operator()(void* ptr) const
    {
        Alloc{}.deallocate(ptr);
    }
};

template <class T, class Alloc>
class data_holder<std::unique_ptr<T[], deleter<Alloc>>>
{
public:
    using value_type = T;

    explicit data_holder(index size) : data_{Alloc{}.allocate(size)} {}

    T* data()
    {
        return data_.get();
    }
    const T* data() const
    {
        return data_.get();
    }

    void swap(data_holder& rhs)
    {
        std::swap(data_, rhs.data_);
    }

private:
    std::unique_ptr<T[], D> data_;
};

template <class T>
class data_holder<T*>
{
public:
    using value_type = T;

    explicit data_holder(index, T* ptr) : data_{ptr} {}

    T* data()
    {
        return data_;
    }
    const T* data() const
    {
        return data_;
    }

    void swap(data_holder& rhs)
    {
        std::swap(data_, rhs.data_);
    }

private:
    T* data_;
};

} // namespace ac
