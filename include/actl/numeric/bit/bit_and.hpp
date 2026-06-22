// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_and_scalar.hpp>

namespace ac {

struct BitAnd : operation_base<BitAnd> {
    using operation_category = bitwise_operation;

    static constexpr auto identity_element = constant<-1>{};

    template<typename L, typename R>
        requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
    friend constexpr auto specialization(BitAnd, type_array<L, R>) noexcept {
        return bit_and_scalar;
    }
};
inline constexpr BitAnd bit_and;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator&(L&& l, R&& r) {
    return bit_and(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator&=(L&& l, R&& r) {
    return bit_and(inout{std::forward<L>(l)}, std::forward<R>(r));
}

} // namespace ac
