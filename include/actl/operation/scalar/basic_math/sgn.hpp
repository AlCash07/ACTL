// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/numeric/comparison/cmp3way.hpp>
#include <actl/operation/scalar/common/arg.hpp>

namespace ac {

struct Sgn : operation_base<Sgn> {
    using operation_category = comparison_operation;

    static constexpr auto formula = cmp3way(x_, 0_c);
};
inline constexpr Sgn sgn;

} // namespace ac
