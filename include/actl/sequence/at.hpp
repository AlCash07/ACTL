// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/operation_base.hpp>

namespace ac {

namespace At {
struct op : operation_base<op> {};
} // namespace At
inline constexpr At::op at;

} // namespace ac
