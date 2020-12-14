// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/all.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/std/vector.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string_view>

using namespace std::string_view_literals;

#define REQUIRE_EQUAL_SETS check_sets
#define REQUIRE_ALMOST_EQUAL_SETS check_almost_equal

namespace ac::math {

template <class T>
struct abs_rel_error : scalar_operation<abs_rel_error<T>, 2, scalar_tag> {
    struct is_policy;

    abs_rel_error(T eps) : eps{eps} {}

    T eps;

    bool eval_scalar(T lhs, T rhs) const {
        T numerator = abs(lhs - rhs);
        T denominator = max(max(abs(lhs), abs(rhs)), T{1});
        return numerator <= eps * denominator;
    }
};

template <class T>
auto apply_policy(Equal, const abs_rel_error<T>& policy) {
    return policy;
}

}  // namespace ac::math

template <class T>
void check_sets(std::vector<T> expected, std::vector<T> actual) {
    sort(expected);
    sort(actual);
    REQUIRE(expected == actual);
}

template <class T, class U, class E>
void check_almost_equal(const T& expected, const U& actual, E eps) {
    REQUIRE(eval(ac::math::equal(ac::math::abs_rel_error<E>{eps})(expected, actual)));
}
