// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>

namespace ac {

namespace Compare3Way {
struct op : operation_base<op> {
    using operation_category = ordering_operation;
};
} // namespace Compare3Way
inline constexpr Compare3Way::op compare_3way;

} // namespace ac
