// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/is_dynamic_range.hpp>

namespace ac {

template<typename T, typename U>
    requires(is_dynamic_range_v<T> && is_dynamic_range_v<U>)
constexpr bool equal_sequences(T const& lhs, U const& rhs) noexcept {
    static_assert(RandomAccessRange<T>);
    static_assert(RandomAccessRange<U>);
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
