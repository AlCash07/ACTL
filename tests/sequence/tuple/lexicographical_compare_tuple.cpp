// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/sequence/tuple/lexicographical_compare_tuple.hpp>

static_assert(0 == ac::compare_3way(std::pair{2, 3}, std::pair{2, 3}));
static_assert(-1 == ac::compare_3way(std::pair{2, 3}, std::pair{3, 2}));
static_assert(1 == ac::compare_3way(std::pair{2, 3}, std::pair{1, 4}));
/* second elements are compared only if the first ones are equal */
static_assert(-1 == ac::compare_3way(std::pair{2, 2}, std::pair{2, 3}));
static_assert(1 == ac::compare_3way(std::pair{2, 4}, std::pair{2, 3}));
