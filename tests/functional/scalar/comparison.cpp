// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/scalar/comparison/all.hpp>
#include "test.hpp"

TEST_CASE("comparison") {
STATIC_ASSERT_WORKAROUND(true == ac::equal(2, 2));
STATIC_ASSERT_WORKAROUND(false == ac::equal(2, 6));
STATIC_ASSERT_WORKAROUND(true == ac::not_equal(2, 6));
STATIC_ASSERT_WORKAROUND(false == ac::not_equal(2, 2));
STATIC_ASSERT_WORKAROUND(true == ac::less(2, 6));
STATIC_ASSERT_WORKAROUND(false == ac::less(2, 2));
STATIC_ASSERT_WORKAROUND(true == ac::greater(6, 2));
STATIC_ASSERT_WORKAROUND(false == ac::greater(2, 6));
STATIC_ASSERT_WORKAROUND(true == ac::less_equal(2, 2));
STATIC_ASSERT_WORKAROUND(false == ac::less_equal(6, 2));
STATIC_ASSERT_WORKAROUND(true == ac::greater_equal(6, 2));
STATIC_ASSERT_WORKAROUND(false == ac::greater_equal(2, 6));
STATIC_ASSERT_WORKAROUND(1 == ac::cmp3way(0, -1));
STATIC_ASSERT_WORKAROUND(-1 == ac::cmp3way(0u, 1u));
}
