// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/comparison/equality.hpp>
#include <actl/numeric/comparison/sgn.hpp>
#include <limits>

static_assert(1 == ac::sgn(std::numeric_limits<double>::max()));
static_assert(-1 == ac::sgn(std::numeric_limits<int>::min()));
static_assert(1 == ac::sgn(std::numeric_limits<unsigned int>::max()));
static_assert(0 == ac::sgn(0));
