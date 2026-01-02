// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/as.hpp>
#include <actl/numeric/arithmetic/additive/subtract.hpp>
#include <actl/numeric/comparison/ordering.hpp>

namespace ac {

struct Compare3WayScalar : operation_base<Compare3WayScalar> {
    // using parent = Compare3Way;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    static constexpr auto formula = as<int>(is_greater) - as<int>(is_less);
};
// AC_REGISTER_OVERLOAD(Compare3WayScalar)
inline constexpr Compare3WayScalar compare3way_scalar;

struct Compare3Way : operation_base<Compare3Way> {
    using operation_category = ordering_operation;

    static constexpr auto formula = compare3way_scalar;
};
inline constexpr Compare3Way compare3way;

} // namespace ac
