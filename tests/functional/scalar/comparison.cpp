// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/comparison.hpp>

TEST_CASE("comparison") {
    static_assert(true == math::equal(2, 2));
    static_assert(false == math::equal(2, 6));
    static_assert(true == math::not_equal(2, 6));
    static_assert(false == math::not_equal(2, 2));
    static_assert(true == math::less(2, 6));
    static_assert(false == math::less(2, 2));
    static_assert(true == math::greater(6, 2));
    static_assert(false == math::greater(2, 6));
    static_assert(true == math::less_equal(2, 2));
    static_assert(false == math::less_equal(6, 2));
    static_assert(true == math::greater_equal(6, 2));
    static_assert(false == math::greater_equal(2, 6));
    static_assert(1 == math::cmp3way(0, -1));
    static_assert(-1 == math::cmp3way(0u, 1u));
}
