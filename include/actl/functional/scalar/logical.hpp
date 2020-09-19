/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/common.hpp>

namespace ac::math {

struct LogicalNot : scalar_operation<LogicalNot, 1, boolean_tag> {
    static constexpr bool eval_scalar(bool x) { return !x; }
};
inline constexpr LogicalNot logical_not;

template <class T>
inline constexpr auto operator ! (T&& x) {
    return logical_not(pass<T>(x));
}

struct LogicalAnd : scalar_operation<LogicalAnd, 2, boolean_tag> {
    struct is_associative;
    struct is_commutative;

    static constexpr bool eval_scalar(bool lhs, bool rhs) { return lhs && rhs; }
};
inline constexpr LogicalAnd logical_and;

template <class T, class U>
inline constexpr auto operator && (T&& lhs, U&& rhs) {
    return logical_and(pass<T>(lhs), pass<U>(rhs));
}

struct LogicalOr : scalar_operation<LogicalOr, 2, boolean_tag> {
    struct is_associative;
    struct is_commutative;

    static constexpr bool eval_scalar(bool lhs, bool rhs) { return lhs || rhs; }
};
inline constexpr LogicalOr logical_or;

template <class T, class U>
inline constexpr auto operator || (T&& lhs, U&& rhs) {
    return logical_or(pass<T>(lhs), pass<U>(rhs));
}

inline constexpr auto logical_implies = !lhs_ || rhs_;

}  // namespace ac::math
