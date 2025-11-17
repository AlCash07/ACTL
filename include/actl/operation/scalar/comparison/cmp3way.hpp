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

struct cmp3way_f : operation_base<cmp3way_f> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = cast<int>(greater) - cast<int>(less);
};
inline constexpr cmp3way_f cmp3way;

} // namespace ac
