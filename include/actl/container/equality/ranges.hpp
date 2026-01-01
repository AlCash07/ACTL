// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/is_dynamic_range.hpp>

namespace ac {

template<typename L, typename R>
    requires(is_dynamic_range_v<L> && is_dynamic_range_v<R>)
constexpr bool is_equal_sequence(L const& l, R const& r) noexcept {
    static_assert(RandomAccessRange<L>);
    static_assert(RandomAccessRange<R>);
    static_assert(noexcept(l.size(), l[0], r.size(), r[0]));
    auto n = l.size();
    if (r.size() != n)
        return false;
    for (decltype(n) i = 0; i != n; ++i)
        if (l[i] != r[i])
            return false;
    return true;
}

} // namespace ac
