// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct Add : operation_base<Add> {
    using operation_category = additive_operation_tag;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
inline constexpr Add add;

} // namespace scalar

struct Add : operation_base<Add> {
    using operation_category = additive_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::add;
};
inline constexpr Add add;

template<typename T>
struct identity_element<Add, T> {
    static constexpr T value() {
        return T{0};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator+(L&& l, R&& r) {
    return add(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator+=(L&& l, R&& r) {
    return add(inout{std::forward<L>(l)}, pass<R>(r));
}

} // namespace ac
