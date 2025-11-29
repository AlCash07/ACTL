// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/subtract.hpp>
#include <actl/operation/scalar/common/cast.hpp>
#include <actl/operation/scalar/comparison/greater.hpp>
#include <actl/operation/scalar/comparison/less.hpp>

namespace ac {

struct Cmp3Way : operation_base<Cmp3Way> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = cast<int>(greater) - cast<int>(less);
};
inline constexpr Cmp3Way cmp3way;

} // namespace ac
