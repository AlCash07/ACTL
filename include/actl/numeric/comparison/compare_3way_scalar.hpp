// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/lifetime/conversion/as.hpp>
#include <actl/numeric/arithmetic/additive/subtract.hpp>
#include <actl/numeric/comparison/ordering.hpp>

namespace ac {

struct Compare3Way;

struct Compare3WayScalar : operation_base<Compare3WayScalar> {
    using parent = Compare3Way;

    static constexpr auto formula = as<int>(is_greater) - as<int>(is_less);
};
inline constexpr Compare3WayScalar compare_3way_scalar;

} // namespace ac
