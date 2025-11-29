// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>
#include <actl/operation/scalar/enable_operators.hpp>

namespace ac {

struct Divide : operation_base<Divide> {
    using operation_category = multiplicative_operation;
};
inline constexpr Divide divide;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator/(L&& l, R&& r) {
    return divide(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator/=(L&& l, R&& r) {
    return divide(inout{std::forward<L>(l)}, pass<R>(r));
}

struct DivideScalars : operation_base<DivideScalars> {
    using operation_category = multiplicative_operation;

    using parent = Divide;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l / r;
    }
};
AC_REGISTER_OVERLOAD(DivideScalars)
inline constexpr DivideScalars divide_scalars;

} // namespace ac
