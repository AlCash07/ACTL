// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>

namespace ac {

namespace Abs {
struct op : operation_base<op> {
    using operation_category = comparison_operation;
};
} // namespace Abs
inline constexpr Abs::op abs;

} // namespace ac
