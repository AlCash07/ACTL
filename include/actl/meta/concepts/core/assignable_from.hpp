// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// @file Extensions for the `std::assignable_from` concept
/// https://en.cppreference.com/w/cpp/concepts/assignable_from

#include <concepts>

namespace ac {

template <class Lhs, class Rhs>
concept TriviallyAssignableFrom =
    std::assignable_from<Lhs, Rhs> && std::is_trivially_assignable_v<Lhs, Rhs>;

template <class Lhs, class Rhs>
concept NothrowAssignableFrom =
    std::assignable_from<Lhs, Rhs> && std::is_nothrow_assignable_v<Lhs, Rhs>;

} // namespace ac
