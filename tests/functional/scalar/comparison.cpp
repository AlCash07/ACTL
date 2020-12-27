// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/comparison.hpp>

static_assert(true == ac::equal(2, 2));
static_assert(false == ac::equal(2, 6));
static_assert(true == ac::not_equal(2, 6));
static_assert(false == ac::not_equal(2, 2));
static_assert(true == ac::less(2, 6));
static_assert(false == ac::less(2, 2));
static_assert(true == ac::greater(6, 2));
static_assert(false == ac::greater(2, 6));
static_assert(true == ac::less_equal(2, 2));
static_assert(false == ac::less_equal(6, 2));
static_assert(true == ac::greater_equal(6, 2));
static_assert(false == ac::greater_equal(2, 6));
static_assert(1 == ac::cmp3way(0, -1));
static_assert(-1 == ac::cmp3way(0u, 1u));
