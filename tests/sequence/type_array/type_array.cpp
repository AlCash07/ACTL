// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/sequence/type_array/type_array.hpp>
#include <type_traits>

static_assert(0 == ac::type_array<>::length);
static_assert(1 == ac::type_array<int>::length);
static_assert(2 == ac::type_array<int, int>::length);
