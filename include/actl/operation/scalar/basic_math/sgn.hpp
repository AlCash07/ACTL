// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/operation/scalar/common/arg.hpp>
#include <actl/operation/scalar/comparison/cmp3way.hpp>

namespace ac {

struct sgn_f : scalar_operation<sgn_f, 1>
{
    using category = comparison_operation_tag;
    using argument_category = arithmetic_tag;

    static constexpr auto formula = cmp3way(x_, 0_c);
};
inline constexpr sgn_f sgn;

} // namespace ac
