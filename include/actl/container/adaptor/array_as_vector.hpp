// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/std/array.hpp>

namespace ac {

/// Array with vector interface.
// TODO: implement full vector interface.
template <class T, size_t N, class Array = std::array<T, N>>
class array_as_vector
{
public:
    using value_type = range_value_t<Array>;
    using reference = range_reference_t<Array>;
    using const_reference = range_reference_t<Array const>;
    using iterator = range_iterator_t<Array>;
    using const_iterator = range_iterator_t<Array const>;
    using size_type = range_size_t<Array>;

    array_as_vector() : end_{array_.begin()} {}

    iterator begin() noexcept
    {
        return array_.begin();
    }

    const_iterator begin() const noexcept
    {
        return array_.begin();
    }

    iterator end() noexcept
    {
        return end_;
    }

    const_iterator end() const noexcept
    {
        return end_;
    }

    size_type size() const noexcept
    {
        return static_cast<size_type>(end() - begin());
    }

    bool empty() const noexcept
    {
        return begin() == end();
    }

    reference operator[](size_type n)
    {
        return array_[n];
    }

    const_reference operator[](size_type n) const
    {
        return array_[n];
    }

    reference front()
    {
        return *begin();
    }

    const_reference front() const
    {
        return *begin();
    }

    reference back()
    {
        return *(end() - 1);
    }

    const_reference back() const
    {
        return *(end() - 1);
    }

    value_type* data() noexcept
    {
        return array_.data();
    }

    value_type const* data() const noexcept
    {
        return array_.data();
    }

    void push_back(T const& v)
    {
        emplace(v);
    }

    void push_back(T&& v)
    {
        emplace(std::move(v));
    }

    template <class... Ts>
    void emplace(Ts&&... args)
    {
        AC_ASSERT(end_ < N);
        *end_++ = T{std::forward<Ts>(args)...};
    }

    void pop_back()
    {
        AC_ASSERT(end_ != array_.begin());
        --end_;
    }

    void clear() noexcept
    {
        end_ = array_.begin();
    }

protected:
    Array array_;
    iterator end_;
};

} // namespace ac
