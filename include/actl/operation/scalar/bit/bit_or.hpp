// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct bit_or_f : scalar_operation<bit_or_f, 2> {
    using operation_category = bitwise_operation_tag;

    template<typename L, typename R>
    static constexpr auto eval_scalar(L l, R r) {
        return l | r;
    }
};
inline constexpr bit_or_f bit_or;

} // namespace scalar

struct bit_or_f : operation_base<bit_or_f> {
    using operation_category = bitwise_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::bit_or;
};
inline constexpr bit_or_f bit_or;

template<typename T>
struct identity_element<bit_or_f, T> {
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

} // namespace ac
