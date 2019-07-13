/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/test.hpp>

TEST("standard") { ASSERT_EQUAL(5.0, norm(point{3, 4}), 1e-12); }

TEST("comparable") { ASSERT_EQUAL(2, sqr(norm(comparable_norm{}, point{1, 1}))); }
