// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

using Vector = std::vector<int>;
static_assert(ac::is_sequence_range_v<Vector>);
static_assert(ac::is_random_access_range_v<Vector>);
static_assert(ac::is_contiguous_range_v<Vector>);
static_assert(!ac::is_associative_range_v<Vector>);
