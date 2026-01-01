// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>
#include <actl/numeric/logic/logical_not.hpp>

namespace ac {

struct IsEqual : operation_base<IsEqual> {
    using operation_category = equality_operation;
};
inline constexpr IsEqual is_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator==(L&& l, R&& r) {
    return is_equal(pass<L>(l), pass<R>(r));
}

inline constexpr auto is_not_equal = !is_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator!=(L&& l, R&& r) {
    return is_not_equal(pass<L>(l), pass<R>(r));
}

struct IsEqualScalar : operation_base<IsEqualScalar> {
    using parent = IsEqual;

    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l == r;
    }
};
AC_REGISTER_OVERLOAD(IsEqualScalar)
inline constexpr IsEqualScalar is_equal_scalar;

} // namespace ac
