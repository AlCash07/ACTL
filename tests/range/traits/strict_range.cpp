// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array.hpp>
#include <actl/range/traits/strict_range.hpp>
#include <array>
#include <vector>

static_assert(!ac::StrictRange<std::tuple<int, int>>);
static_assert(ac::StrictRange<std::vector<int>>);

using Array = std::array<int, 3>;
static_assert(ac::Range<Array> && ac::Tuple<Array>);
static_assert(ac::StrictRange<Array>);

using StaticArray = ac::static_array<int, 2, 3>;
static_assert(ac::Range<StaticArray> && ac::Tuple<StaticArray>);
static_assert(!ac::StrictRange<StaticArray>);
