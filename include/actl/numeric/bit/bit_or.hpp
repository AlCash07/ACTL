// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>
#include <actl/operation/scalar/enable_operators.hpp>

namespace ac {

struct BitOr : operation_base<BitOr> {
    using operation_category = bitwise_operation;
};
inline constexpr BitOr bit_or;

template<typename T>
struct identity_element<BitOr, T> {
    static constexpr T value() {
        return T{0};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator|(L&& l, R&& r) {
    return bit_or(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator|=(L&& l, R&& r) {
    return bit_or(inout{std::forward<L>(l)}, pass<R>(r));
}

struct BitOrScalar : operation_base<BitOrScalar> {
    using parent = BitOr;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<std::integral L, std::integral R>
    static constexpr auto evaluate(L l, R r) {
        return l | r;
    }
};
AC_REGISTER_OVERLOAD(BitOrScalar)
inline constexpr BitOrScalar bit_or_scalar;

} // namespace ac
