// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/sequence/tuple/is_equal_tuple.hpp>

static_assert(ac::is_equal(std::pair{2, 3}, std::pair{2, 3}));
static_assert(!ac::is_equal(std::pair{2, 3}, std::pair{1, 3}));
static_assert(!ac::is_equal(std::pair{2, 3}, std::pair{2, 4}));
