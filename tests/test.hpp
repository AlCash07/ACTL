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

template<typename T>
struct abs_rel_error : scalar_operation<abs_rel_error<T>, 2> {
    using operation_category = scalar_operation_tag;

    struct is_policy;

    abs_rel_error(T eps) : eps{eps} {}

    T eps;

    bool eval_scalar(T l, T r) const {
        T numerator = abs(l - r);
        T denominator = max(max(std::abs(l), std::abs(r)), T{1});
        return numerator <= eps * denominator;
    }
};

template<typename T>
auto apply_policy(scalar::equal_f, abs_rel_error<T> const& policy) {
    return policy;
}

} // namespace ac

template<typename T>
void check_sets(std::vector<T> expected, std::vector<T> actual) {
    sort(expected);
    sort(actual);
    CHECK(expected == actual);
}

template<typename T, typename U, typename E>
void check_near(T const& expected, U const& actual, E eps) {
    CHECK((ac::equal | ac::abs_rel_error<E>{eps})(expected, actual));
}

template<typename T, typename U>
void check_equal(T const& expected, U const& actual) {
    CHECK(ac::equal(expected, actual));
}

template<typename T, typename U>
void check_not_equal(T const& not_expected, U const& actual) {
    CHECK_FALSE(ac::equal(not_expected, actual));
}
