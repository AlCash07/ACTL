// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/operation/operation_base.hpp>

namespace ac {

struct At : operation_base<At> {};
inline constexpr At at;

} // namespace ac
