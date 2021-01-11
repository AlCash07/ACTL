// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/fold/fold.hpp>
#include <actl/functional/scalar/arithmetic/add.hpp>
#include <actl/functional/scalar/arithmetic/mul.hpp>
#include <actl/functional/scalar/basic_math/max.hpp>
#include <actl/functional/scalar/basic_math/min.hpp>

namespace ac {

constexpr auto sum = fold(add);
constexpr auto product = fold(mul);

constexpr auto max_element = fold(max);
constexpr auto min_element = fold(min);

}  // namespace ac
