// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/subtract.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <actl/operation/scalar/common/cast.hpp>

namespace ac {

struct Cmp3Way : operation_base<Cmp3Way> {
    using operation_category = ordering_operation;

    static constexpr auto formula = cast<int>(is_greater) - cast<int>(is_less);
};
inline constexpr Cmp3Way cmp3way;

} // namespace ac
