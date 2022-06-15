// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/meta/static_size.hpp>

namespace ac {

template <size_t N>
class size_holder
{
public:
    static_assert(N >= 0);

    constexpr size_holder(size_t n [[maybe_unused]])
    {
        AC_ASSERT(n == N);
    }

    static constexpr size_t size()
    {
        return N;
    }
};

template <>
class size_holder<dynamic_size>
{
public:
    constexpr size_holder(size_t n) : size_{n} {}

    constexpr size_t size() const
    {
        return size_;
    }

private:
    size_t size_;
};

} // namespace ac
