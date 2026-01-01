// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Subtract : operation_base<Subtract> {
    using operation_category = additive_operation;
};
inline constexpr Subtract subtract;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator-(L&& l, R&& r) {
    return subtract(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator-=(L&& l, R&& r) {
    return subtract(inout{std::forward<L>(l)}, pass<R>(r));
}

struct SubtractScalars : operation_base<SubtractScalars> {
    using parent = Subtract;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l - r;
    }
};
AC_REGISTER_OVERLOAD(SubtractScalars)
inline constexpr SubtractScalars subtract_scalars;

} // namespace ac
