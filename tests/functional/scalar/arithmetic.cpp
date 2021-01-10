// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/scalar/arithmetic/all.hpp>
#include "test.hpp"

static_assert(-6 == ac::neg(6));
static_assert(2 == ac::neg(-2));
static_assert(8 == ac::add(6, 2));
static_assert(4 == ac::sub(6, 2));
static_assert(12 == ac::mul(6, 2));
static_assert(3 == ac::div(7, 2));
