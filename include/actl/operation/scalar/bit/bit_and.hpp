// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct BitAnd : operation_base<BitAnd> {
    using operation_category = bitwise_operation_tag;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l & r;
    }
};
inline constexpr BitAnd bit_and;

} // namespace scalar

struct BitAnd : operation_base<BitAnd> {
    using operation_category = bitwise_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::bit_and;
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
    return bit_and(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator&=(L&& l, R&& r) {
    return bit_and(inout{std::forward<L>(l)}, pass<R>(r));
}

} // namespace ac
