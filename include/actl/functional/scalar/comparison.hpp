/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/arithmetic.hpp>
#include <actl/functional/scalar/logical.hpp>

namespace ac::math {

template <class Derived>
struct comparison_operation : scalar_operation<Derived, 2, scalar_tag> {};

template <class T>
inline constexpr bool is_comparison_v = is_template_base_of_v<comparison_operation, T>;

struct Equal : comparison_operation<Equal> {
    struct is_commutative;

    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs == rhs;
    }
};
inline constexpr Equal equal;

template <class T, class U>
inline constexpr auto operator == (T&& lhs, U&& rhs) {
    return equal(pass<T>(lhs), pass<U>(rhs));
}

struct NotEqual : comparison_operation<NotEqual> {
    struct is_commutative;

    static constexpr auto formula = !equal;
};
inline constexpr NotEqual not_equal;

template <class T, class U>
inline constexpr auto operator != (T&& lhs, U&& rhs) {
    return not_equal(pass<T>(lhs), pass<U>(rhs));
}

struct Less : comparison_operation<Less> {
    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs < rhs;
    }
};
inline constexpr Less less;

template <class T, class U>
inline constexpr auto operator < (T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

struct Greater : comparison_operation<Greater> {
    static constexpr auto formula = rhs_ < lhs_;
};
inline constexpr Greater greater;

template <class T, class U>
inline constexpr auto operator > (T&& lhs, U&& rhs) {
    return greater(pass<U>(lhs), pass<T>(rhs));
}

struct LessEqual : comparison_operation<LessEqual> {
    static constexpr auto formula = !greater;
};
inline constexpr LessEqual less_equal;

template <class T, class U>
inline constexpr auto operator <= (T&& lhs, U&& rhs) {
    return less_equal(pass<T>(lhs), pass<U>(rhs));
}

struct GreaterEqual : comparison_operation<GreaterEqual> {
    static constexpr auto formula = !less;
};
inline constexpr GreaterEqual greater_equal;

template <class T, class U>
inline constexpr auto operator >= (T&& lhs, U&& rhs) {
    return greater_equal(pass<T>(lhs), pass<U>(rhs));
}

struct Cmp3Way : comparison_operation<Cmp3Way> {
    static constexpr auto formula = cast<int>(greater) - cast<int>(less);
};
inline constexpr Cmp3Way cmp3way;

}  // namespace ac::math
