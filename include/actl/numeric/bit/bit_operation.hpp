// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/core/scalar_operation.hpp>
#include <actl/operation/enable_operators.hpp>

namespace ac {

struct bit_operation : scalar_operation {};
struct bitwise_operation : bit_operation {};

} // namespace ac
