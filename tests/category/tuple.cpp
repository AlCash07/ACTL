// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/category/tuple.hpp>
#include <actl/std/array.hpp>
#include <actl/std/tuple.hpp>
#include <actl/std/utility.hpp>
#include "test.hpp"

static_assert(ac::is_tuple_v<std::pair<int, int>>);
static_assert(ac::is_tuple_v<std::tuple<int, int, int>>);
// Array isn't a tuple even though std::tuple_size is defined for it, because it's a range.
static_assert(!ac::is_tuple_v<std::array<int, 2>>);
