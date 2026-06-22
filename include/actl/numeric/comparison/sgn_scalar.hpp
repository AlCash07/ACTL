// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/compare_3way.hpp>
#include <actl/numeric/constant_literals.hpp>
#include <actl/operation/arg.hpp>

namespace ac {

struct Sgn;

struct SgnScalar : operation_base<SgnScalar> {
    using parent = Sgn;

    static constexpr auto formula = compare_3way(t_, 0_c);
};
inline constexpr SgnScalar sgn_scalar;

} // namespace ac
