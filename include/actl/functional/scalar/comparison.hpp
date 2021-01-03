// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/arithmetic.hpp>
#include <actl/functional/scalar/logical.hpp>
#include <actl/traits/is_template_base_of.hpp>

namespace ac {

template <class Derived>
struct comparison_operation : scalar_operation<Derived, 2, scalar_tag> {};

template <class T>
constexpr bool is_comparison_v = is_template_base_of_v<comparison_operation, T>;

struct equal_t : comparison_operation<equal_t> {
    struct is_commutative;

    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs == rhs;
    }
};
constexpr equal_t equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator==(T&& lhs, U&& rhs) {
    return equal(pass<T>(lhs), pass<U>(rhs));
}

struct non_equal_t : comparison_operation<non_equal_t> {
    struct is_commutative;

    static constexpr auto formula = !equal;
};
constexpr non_equal_t not_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator!=(T&& lhs, U&& rhs) {
    return not_equal(pass<T>(lhs), pass<U>(rhs));
}

struct less_t : comparison_operation<less_t> {
    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs < rhs;
    }
};
constexpr less_t less;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator<(T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

struct greater_t : comparison_operation<greater_t> {
    static constexpr auto formula = rhs_ < lhs_;
};
constexpr greater_t greater;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator>(T&& lhs, U&& rhs) {
    return greater(pass<T>(lhs), pass<U>(rhs));
}

struct less_equal_t : comparison_operation<less_equal_t> {
    static constexpr auto formula = !greater;
};
constexpr less_equal_t less_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator<=(T&& lhs, U&& rhs) {
    return less_equal(pass<T>(lhs), pass<U>(rhs));
}

struct greater_equal_t : comparison_operation<greater_equal_t> {
    static constexpr auto formula = !less;
};
constexpr greater_equal_t greater_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator>=(T&& lhs, U&& rhs) {
    return greater_equal(pass<T>(lhs), pass<U>(rhs));
}

struct cmp3way_t : comparison_operation<cmp3way_t> {
    static constexpr auto formula = cast<int>(greater) - cast<int>(less);
};
constexpr cmp3way_t cmp3way;

}  // namespace ac
