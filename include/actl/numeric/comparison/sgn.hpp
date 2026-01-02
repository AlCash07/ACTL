// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/numeric/comparison/compare3way.hpp>
#include <actl/operation/scalar/common/arg.hpp>

namespace ac {

struct Sgn : operation_base<Sgn> {
    using operation_category = comparison_operation;
};
inline constexpr Sgn sgn;

struct SgnScalar : operation_base<SgnScalar> {
    using parent = Sgn;

    template<typename T>
    static constexpr bool requirement = std::is_arithmetic_v<T>;

    static constexpr auto formula = compare3way(x_, 0_c);
};
AC_REGISTER_OVERLOAD(SgnScalar)
inline constexpr SgnScalar sgn_scalar;

} // namespace ac
