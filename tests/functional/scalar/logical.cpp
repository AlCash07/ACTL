// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/scalar/logical/all.hpp>
#include "test.hpp"

TEST_CASE("logical") {
STATIC_ASSERT_WORKAROUND(true == ac::logical_not(false));
STATIC_ASSERT_WORKAROUND(false == ac::logical_not(true));
STATIC_ASSERT_WORKAROUND(true == ac::logical_and(true, true));
STATIC_ASSERT_WORKAROUND(false == ac::logical_and(true, false));
STATIC_ASSERT_WORKAROUND(true == ac::logical_or(true, false));
STATIC_ASSERT_WORKAROUND(false == ac::logical_or(false, false));
STATIC_ASSERT_WORKAROUND(true == ac::logical_implies(true, true));
STATIC_ASSERT_WORKAROUND(true == ac::logical_implies(false, true));
STATIC_ASSERT_WORKAROUND(false == ac::logical_implies(true, false));
}
