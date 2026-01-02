// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/arg.hpp>
#include <actl/numeric/comparison/comparison_operation.hpp>
#include <actl/numeric/logic/logical_not.hpp>

namespace ac {

struct IsLessScalar : operation_base<IsLessScalar> {
    // using parent = IsLess;
    using operation_category = ordering_operation;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l < r;
    }
};
// AC_REGISTER_OVERLOAD(IsLessScalar)
inline constexpr IsLessScalar is_less_scalar;

struct IsLess : operation_base<IsLess> {
    using operation_category = ordering_operation;

    // TODO: remove this formula.
    static constexpr auto formula = is_less_scalar;
};
inline constexpr IsLess is_less;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<(L&& l, R&& r) {
    return is_less(pass<L>(l), pass<R>(r));
}

struct IsGreater : operation_base<IsGreater> {
    using operation_category = ordering_operation;

    static constexpr auto formula = r_ < l_;
};
// TODO: after fixing the tests this should be simply is_greater = r_ < l_;
inline constexpr IsGreater is_greater;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator>(L&& l, R&& r) {
    return is_greater(pass<L>(l), pass<R>(r));
}

inline constexpr auto is_less_or_equal = !is_greater;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<=(L&& l, R&& r) {
    return is_less_or_equal(pass<L>(l), pass<R>(r));
}

inline constexpr auto is_greater_or_equal = !is_less;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator>=(L&& l, R&& r) {
    return is_greater_or_equal(pass<L>(l), pass<R>(r));
}

} // namespace ac
