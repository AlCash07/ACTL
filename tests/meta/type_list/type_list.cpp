// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/type_list/type_list.hpp>
#include <type_traits>

static_assert(0 == ac::type_list<>::length);
static_assert(1 == ac::type_list<int>::length);
static_assert(2 == ac::type_list<int, int>::length);
