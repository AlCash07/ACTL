// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/common/arg.hpp>
#include <actl/operation/scalar/logical/logical_not.hpp>
#include <actl/operation/scalar/logical/logical_or.hpp>

namespace ac {

inline constexpr auto logical_implies = !lhs_ || rhs_;

} // namespace ac
