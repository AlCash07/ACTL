// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/subtract_scalar.hpp>

namespace ac {

struct Subtract : operation_base<Subtract> {
    using operation_category = additive_operation;

    template<typename L, typename R>
        requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
    friend constexpr auto specialization(Subtract, type_array<L, R>) noexcept {
        return subtract_scalar;
    }
};
inline constexpr Subtract subtract;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator-(L&& l, R&& r) {
    return subtract(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator-=(L&& l, R&& r) {
    return subtract(inout{std::forward<L>(l)}, std::forward<R>(r));
}

} // namespace ac
