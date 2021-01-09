// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/basic_math.hpp>
#include <limits>

static_assert(1 == ac::sgn(std::numeric_limits<double>::max()));
static_assert(-1 == ac::sgn(std::numeric_limits<int>::min()));
static_assert(1 == ac::sgn(std::numeric_limits<unsigned int>::max()));
static_assert(0 == ac::sgn(0));

static_assert(36 == ac::sqr(6));
static_assert(4 == ac::sqr(-2));
