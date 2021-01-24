// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/arithmetic/sub.hpp>
#include <actl/operation/scalar/common/cast.hpp>
#include <actl/operation/scalar/comparison/greater.hpp>
#include <actl/operation/scalar/comparison/less.hpp>

namespace ac {

struct cmp3way_t : scalar_operation<cmp3way_t, 2> {
    using category = ordering_operation_tag;
    using argument_category = scalar_tag;

    static constexpr auto formula = cast<int>(greater) - cast<int>(less);
};
constexpr cmp3way_t cmp3way;

} // namespace ac
