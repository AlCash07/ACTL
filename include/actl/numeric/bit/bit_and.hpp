// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

struct BitAnd : operation_base<BitAnd> {
    using operation_category = bitwise_operation;
};
inline constexpr BitAnd bit_and;

template<typename T>
struct identity_element<BitAnd, T> {
    static constexpr T value() {
        return T{-1};
    }
};

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

struct BitAndScalar : operation_base<BitAndScalar> {
    using parent = BitAnd;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<std::integral L, std::integral R>
    static constexpr auto evaluate(L l, R r) {
        return l & r;
    }
};
AC_REGISTER_OVERLOAD(BitAndScalar)
inline constexpr BitAndScalar bit_and_scalar;

} // namespace ac
