/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/scalar.hpp>
#include <actl/test.hpp>
#include <limits>

TEST("inplace parameter") {
    int x = 4;
    ASSERT_EQUAL(5, math::max(5, inplace(x)));
    ASSERT_EQUAL(5, x);
    ASSERT_EQUAL(5, math::max(inplace(x), 3));
    ASSERT_EQUAL(5, x);
}

TEST("output parameter") {
    int res{};
    ASSERT_EQUAL(6, math::add(out(res), 2, 4));
    ASSERT_EQUAL(6, res);
}

TEST("arithmetics") {
    ASSERT_EQUAL(-6, math::neg(6));
    ASSERT_EQUAL(2, math::neg(-2));
    ASSERT_EQUAL(8, math::add(6, 2));
    ASSERT_EQUAL(3, math::div(6, 2));
    ASSERT_EQUAL(12, math::mul(6, 2));
    ASSERT_EQUAL(4, math::sub(6, 2));
}

TEST("math::cmp3way") {
    ASSERT_EQUAL(1, math::cmp3way(0, -1));
    ASSERT_EQUAL(-1, math::cmp3way(0u, 1u));
}

TEST("logical") {
    ASSERT_EQUAL(true, math::logical_not(false));
    ASSERT_EQUAL(false, math::logical_not(true));
    ASSERT_EQUAL(true, math::logical_and(true, true));
    ASSERT_EQUAL(false, math::logical_and(true, false));
    ASSERT_EQUAL(true, math::logical_or(true, false));
    ASSERT_EQUAL(false, math::logical_or(false, false));
}

TEST("bit") {
    ASSERT_EQUAL(-1, math::bit_not(0));
    ASSERT_EQUAL(1, math::bit_and(3, 5));
    ASSERT_EQUAL(7, math::bit_or(3, 5));
}

TEST("math::sgn") {
    ASSERT_EQUAL(1, math::sgn(std::numeric_limits<int>::max()));
    ASSERT_EQUAL(-1, math::sgn(std::numeric_limits<int>::min()));
    ASSERT_EQUAL(1, math::sgn(std::numeric_limits<unsigned int>::max()));
    ASSERT_EQUAL(0, math::sgn(0));
}

TEST("math::sqr") {
    ASSERT_EQUAL(36, math::sqr(6));
    ASSERT_EQUAL(4, math::sqr(-2));
}

TEST("math::common") {
    using one = std::integral_constant<int, 1>;
    static_assert(std::is_same_v<none, decltype(eval(math::common(none{}, none{}, none{})))>);
    static_assert(1 == eval(math::common(none{}, one{}, none{})).value);
    static_assert(1 == eval(math::common(one{})).value);
    static_assert(1 == eval(math::common(one{}, one{}, one{})).value);
    int x = 1;
    static_assert(std::is_same_v<one, decltype(eval(math::common(x, one{}, x)))>);
    ASSERT_EQUAL(2, math::common(2, 2, 2));
}
