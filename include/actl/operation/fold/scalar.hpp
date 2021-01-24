// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/fold/fold.hpp>
#include <actl/operation/scalar/arithmetic/add.hpp>
#include <actl/operation/scalar/arithmetic/mul.hpp>
#include <actl/operation/scalar/basic_math/max.hpp>
#include <actl/operation/scalar/basic_math/min.hpp>

namespace ac {

inline constexpr auto sum = fold(add);
inline constexpr auto product = fold(mul);

inline constexpr auto max_element = fold(max);
inline constexpr auto min_element = fold(min);

} // namespace ac
