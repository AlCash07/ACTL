// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/arithmetic/all.hpp>

/* additive */
static_assert(-6 == ac::negate(6));
static_assert(2 == ac::negate(-2));
static_assert(8 == ac::add(6, 2));
static_assert(4 == ac::subtract(6, 2));
/* multiplicative */
static_assert(12 == ac::multiply(6, 2));
static_assert(3 == ac::divide(7, 2));
/* exponential */
static_assert(36 == ac::squared(6));
static_assert(4 == ac::squared(-2));
