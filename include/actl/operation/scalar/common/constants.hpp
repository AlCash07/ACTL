// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

constexpr std::integral_constant<int, 0> zero_;
constexpr std::integral_constant<int, 1> one_;
constexpr std::integral_constant<int, 2> two_;

} // namespace ac
