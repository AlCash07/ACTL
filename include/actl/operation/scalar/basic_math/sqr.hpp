// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/arithmetic/mul.hpp>
#include <actl/operation/scalar/common/arg.hpp>

namespace ac {

struct sqr_t : scalar_operation<sqr_t, 1> {
    using category = multiplicative_operation_tag;
    using argument_category = arithmetic_tag;

    static constexpr auto formula = mul(x_, x_);
};
constexpr sqr_t sqr;

}  // namespace ac
