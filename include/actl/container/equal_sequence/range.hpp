// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_traits.hpp>
#include <actl/range/is_dynamic_range.hpp>

namespace ac {

template <
    class T,
    class U,
    enable_int_if<is_dynamic_range_v<T> && is_dynamic_range_v<U>> = 0>
constexpr bool equal_sequence(const T& lhs, const U& rhs) noexcept
{
    static_assert(is_random_access_range_v<T>);
    static_assert(is_random_access_range_v<U>);
    static_assert(noexcept(lhs.size(), lhs[0], rhs.size(), rhs[0]));
    auto n = lhs.size();
    if (rhs.size() != n)
        return false;
    for (decltype(n) i = 0; i != n; ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

} // namespace ac
