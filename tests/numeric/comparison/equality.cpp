// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/comparison/equality.hpp>

static_assert(true == ac::is_equal(2, 2));
static_assert(false == ac::is_equal(2, 6));

static_assert(true == ac::is_not_equal(2, 6));
static_assert(false == ac::is_not_equal(2, 2));
