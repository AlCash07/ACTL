// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/all.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/range/irange.hpp>
#include <actl/range/operation/comparison.hpp>
#include <actl/std/vector.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <string_view>

using std::string_view_literals::operator""sv;
using namespace ac;

#define CHECK_EQUAL check_equal
#define CHECK_NOT_EQUAL check_not_equal
#define CHECK_EQUAL_SETS check_sets
#define CHECK_NEAR check_near

namespace ac {

template <class T>
struct abs_rel_error : scalar_operation<abs_rel_error<T>, 2>
{
    using category = scalar_operation_tag;
    using argument_category = scalar_tag;

    struct is_policy;

    abs_rel_error(T eps) : eps{eps} {}

    T eps;

    bool eval_scalar(T lhs, T rhs) const
    {
        T numerator = abs(lhs - rhs);
        T denominator = max(max(std::abs(lhs), std::abs(rhs)), T{1});
        return numerator <= eps * denominator;
    }
};

template <class T>
auto apply_policy(scalar::equal_f, const abs_rel_error<T>& policy)
{
    return policy;
}

} // namespace ac

template <class T>
void check_sets(std::vector<T> expected, std::vector<T> actual)
{
    sort(expected);
    sort(actual);
    CHECK(expected == actual);
}

template <class T, class U, class E>
void check_near(const T& expected, const U& actual, E eps)
{
    CHECK((ac::equal | ac::abs_rel_error<E>{eps})(expected, actual));
}

template <class T, class U>
void check_equal(const T& expected, const U& actual)
{
    CHECK(ac::equal(expected, actual));
}

template <class T, class U>
void check_not_equal(const T& not_expected, const U& actual)
{
    CHECK_FALSE(ac::equal(not_expected, actual));
}
