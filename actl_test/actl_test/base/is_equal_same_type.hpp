// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template<typename L, typename R>
constexpr bool is_equal_same_type(L const& l, R const& r) noexcept(
    noexcept(l == r)
) {
    static_assert(std::is_same_v<L, R>);
    return l == r;
}

} // namespace ac
