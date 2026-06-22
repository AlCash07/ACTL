// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/is_less_scalar.hpp>
#include <actl/numeric/logic/logical_not.hpp>
#include <actl/operation/arg.hpp>

namespace ac {

struct IsLess : operation_base<IsLess> {
    using operation_category = ordering_operation;

    template<typename L, typename R>
        requires(
            (is_constant_v<L> || std::is_arithmetic_v<L>) &&
            (is_constant_v<R> || std::is_arithmetic_v<R>)
        )
    friend constexpr auto specialization(IsLess, type_array<L, R>) noexcept {
        return is_less_scalar;
    }
};
inline constexpr IsLess is_less;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<(L&& l, R&& r) {
    return is_less(std::forward<L>(l), std::forward<R>(r));
}

inline constexpr auto is_greater = r_ < l_;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator>(L&& l, R&& r) {
    return is_greater(std::forward<L>(l), std::forward<R>(r));
}

inline constexpr auto is_less_or_equal = !is_greater;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<=(L&& l, R&& r) {
    return is_less_or_equal(std::forward<L>(l), std::forward<R>(r));
}

inline constexpr auto is_greater_or_equal = !is_less;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator>=(L&& l, R&& r) {
    return is_greater_or_equal(std::forward<L>(l), std::forward<R>(r));
}

} // namespace ac
