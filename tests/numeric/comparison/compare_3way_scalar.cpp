// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/comparison/compare_3way_scalar.hpp>

static_assert(1 == ac::compare_3way(0, -1));
static_assert(-1 == ac::compare_3way(0u, 1u));
