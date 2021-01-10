// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/common/arg.hpp>
#include <actl/functional/scalar/common/constants.hpp>
#include <actl/functional/scalar/comparison/cmp3way.hpp>

namespace ac {

struct sgn_t : scalar_operation<sgn_t, 1, scalar_tag> {
    static constexpr auto formula = cmp3way(x_, zero_);
};
constexpr sgn_t sgn;

}  // namespace ac
