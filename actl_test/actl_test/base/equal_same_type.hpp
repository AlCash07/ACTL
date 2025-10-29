// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template<typename T, typename U>
constexpr bool equal_same_type(T const& lhs, U const& rhs) noexcept(
    noexcept(lhs == rhs)
) {
    static_assert(std::is_same_v<T, U>);
    return lhs == rhs;
}

} // namespace ac
