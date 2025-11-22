// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct Div : operation_base<Div> {
    using operation_category = multiplicative_operation_tag;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l / r;
    }
};
inline constexpr Div div;

} // namespace scalar

struct Div : operation_base<Div> {
    using operation_category = multiplicative_operation_tag;

    static constexpr auto formula = scalar::div;
};
inline constexpr Div div;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator/(L&& l, R&& r) {
    return div(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator/=(L&& l, R&& r) {
    return div(inout{std::forward<L>(l)}, pass<R>(r));
}

} // namespace ac
