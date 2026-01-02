// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/comparison/ordering.hpp>

static_assert(true == ac::is_less(2, 6));
static_assert(false == ac::is_less(2, 2));

static_assert(true == ac::is_greater(6, 2));
static_assert(false == ac::is_greater(2, 6));

static_assert(true == ac::is_less_or_equal(2, 2));
static_assert(false == ac::is_less_or_equal(6, 2));

static_assert(true == ac::is_greater_or_equal(6, 2));
static_assert(false == ac::is_greater_or_equal(2, 6));
