// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/is_dynamic_range.hpp>
#include <vector>

static_assert(ac::is_dynamic_range_v<std::vector<int>>);
static_assert(ac::is_dynamic_range_v<std::vector<int> const&>);

static_assert(ac::Range<int[3]>);
static_assert(!ac::is_dynamic_range_v<int[3]>);
static_assert(ac::Range<int (&)[3]>);
static_assert(!ac::is_dynamic_range_v<int (&)[3]>);

static_assert(!ac::is_dynamic_range_v<int>);
