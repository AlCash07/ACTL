/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/scalar/logical.hpp>
#include <actl/test.hpp>

TEST("logical") {
    static_assert(true == math::logical_not(false));
    static_assert(false == math::logical_not(true));
    static_assert(true == math::logical_and(true, true));
    static_assert(false == math::logical_and(true, false));
    static_assert(true == math::logical_or(true, false));
    static_assert(false == math::logical_or(false, false));
    static_assert(true == math::logical_implies(true, true));
    static_assert(true == math::logical_implies(false, true));
    static_assert(false == math::logical_implies(true, false));
}
