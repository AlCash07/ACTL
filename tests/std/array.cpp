// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/is_associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/array.hpp>

using Array = std::array<int, 2>;
static_assert(ac::is_container_v<Array>);
static_assert(ac::is_sequence_range_v<Array>);
static_assert(ac::RandomAccessRange<Array>);
static_assert(ac::ContiguousRange<Array>);
static_assert(!ac::is_associative_range_v<Array>);
