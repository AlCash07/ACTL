// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/core/scalar_operation.hpp>
#include <actl/operation/enable_operators.hpp>

namespace ac {

// clang-format off
struct arithmetic_operation     : scalar_operation {};
struct additive_operation       : arithmetic_operation {};
struct multiplicative_operation : arithmetic_operation {};
struct exponential_operation    : arithmetic_operation {};
// clang-format on

} // namespace ac
