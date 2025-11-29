// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/multiplicative/multiply.hpp>
#include <actl/operation/scalar/common/arg.hpp>

namespace ac {

struct Squared : operation_base<Squared> {
    using operation_category = exponential_operation;

    static constexpr auto formula = x_ * x_;
};
inline constexpr Squared squared;

} // namespace ac
