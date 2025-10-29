// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template<typename Lhs, typename Rhs>
concept TriviallyAssignableFrom =
    std::assignable_from<Lhs, Rhs> && std::is_trivially_assignable_v<Lhs, Rhs>;

template<typename Lhs, typename Rhs>
concept NothrowAssignableFrom =
    std::assignable_from<Lhs, Rhs> && std::is_nothrow_assignable_v<Lhs, Rhs>;

} // namespace ac
