// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/multiplicative/divide_scalar.hpp>

namespace ac {

struct Divide : operation_base<Divide> {
    using operation_category = multiplicative_operation;

    template<typename L, typename R>
        requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
    friend constexpr auto specialization(Divide, type_array<L, R>) noexcept {
        return divide_scalar;
    }
};
inline constexpr Divide divide;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator/(L&& l, R&& r) {
    return divide(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator/=(L&& l, R&& r) {
    return divide(inout{std::forward<L>(l)}, std::forward<R>(r));
}

} // namespace ac
