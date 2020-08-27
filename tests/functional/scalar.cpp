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
    ASSERT_EQUAL(2, inplace(x) -= 3);
    ASSERT_EQUAL(2, x);
}

TEST("output parameter") {
    int res{};
    ASSERT_EQUAL(6, math::add(out(res), 2, 4));
    ASSERT_EQUAL(6, res);
    out(res) = math::add(2, res);
    ASSERT_EQUAL(8, res);
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

TEST("comparison") {
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

TEST("derived from comparison") {
    static_assert(1 == math::sgn(std::numeric_limits<double>::max()));
    static_assert(-1 == math::sgn(std::numeric_limits<int>::min()));
    static_assert(1 == math::sgn(std::numeric_limits<unsigned int>::max()));
    static_assert(0 == math::sgn(0));
}

TEST("arithmetic") {
    static_assert(-6 == math::neg(6));
    static_assert(2 == math::neg(-2));
    static_assert(8 == math::add(6, 2));
    static_assert(4 == math::sub(6, 2));
    static_assert(12 == math::mul(6, 2));
    static_assert(36 == math::sqr(6));
    static_assert(4 == math::sqr(-2));
    static_assert(3 == math::div(7, 2));
}

TEST("bit") {
    static_assert(-2 == math::bit_not(1));
    static_assert(1 == math::bit_and(3, 5));
    static_assert(7 == math::bit_or(3, 5));
    static_assert(6 == math::bit_xor(3, 5));
}
