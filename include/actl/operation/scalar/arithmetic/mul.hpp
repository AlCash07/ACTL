// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct mul_f : scalar_operation<mul_f, 2> {
    using operation_category = multiplicative_operation_tag;

    template<typename L, typename R>
    static constexpr auto eval_scalar(L l, R r) {
        return l * r;
    }
};
inline constexpr mul_f mul;

} // namespace scalar

struct mul_f : operation<mul_f> {
    using operation_category = multiplicative_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::mul;
};
inline constexpr mul_f mul;

template<typename T>
struct identity_element<mul_f, T> {
    static constexpr T value() {
        return T{1};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator*(L&& l, R&& r) {
    return mul(pass<L>(l), pass<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator*=(L&& l, R&& r) {
    return mul(inout{std::forward<L>(l)}, pass<R>(r));
}

} // namespace ac
