// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/arithmetic/mul.hpp>
#include <actl/operation/scalar/common/arg.hpp>

namespace ac {

struct sqr_f : scalar_operation<sqr_f, 1>
{
    using operation_category = multiplicative_operation_tag;

    static constexpr auto formula = x_ * x_;
};
inline constexpr sqr_f sqr;

} // namespace ac
