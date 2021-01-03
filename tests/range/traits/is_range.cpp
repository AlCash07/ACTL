// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/traits/is_range.hpp>
#include <actl/std/list.hpp>

static_assert(!ac::is_range_v<int>);
static_assert(ac::is_range_v<int[4]>);
static_assert(ac::is_range_v<const int[4]>);
static_assert(ac::is_range_v<std::list<int>>);
