// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/bit.hpp>

static_assert(-2 == ac::bit_not(1));
static_assert(1 == ac::bit_and(3, 5));
static_assert(7 == ac::bit_or(3, 5));
static_assert(6 == ac::bit_xor(3, 5));
