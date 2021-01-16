// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/scalar/basic_math/all.hpp>
#include <limits>
#include "test.hpp"

TEST_CASE("basic_math") {
STATIC_ASSERT_WORKAROUND(1 == ac::sgn(std::numeric_limits<double>::max()));
STATIC_ASSERT_WORKAROUND(-1 == ac::sgn(std::numeric_limits<int>::min()));
STATIC_ASSERT_WORKAROUND(1 == ac::sgn(std::numeric_limits<unsigned int>::max()));
STATIC_ASSERT_WORKAROUND(0 == ac::sgn(0));

STATIC_ASSERT_WORKAROUND(36 == ac::sqr(6));
STATIC_ASSERT_WORKAROUND(4 == ac::sqr(-2));
}
