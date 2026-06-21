// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Add : operation_base<Add> {
    using operation_category = additive_operation;

    static constexpr auto identity_element = 0_c;
};
inline constexpr Add add;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator+(L&& l, R&& r) {
    return add(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator+=(L&& l, R&& r) {
    return add(inout{std::forward<L>(l)}, std::forward<R>(r));
}

struct AddInteger : operation_base<AddInteger> {
    using parent = Add;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool match = std::integral<L> && std::integral<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
AC_REGISTER_SPECIALIZATION(AddInteger)
inline constexpr AddInteger add_integer;

struct AddFloat : operation_base<AddFloat> {
    using parent = Add;

    // floating point addition is neither associative nor commutative

    template<typename L, typename R>
    static constexpr bool match =
        std::floating_point<L> && std::floating_point<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
AC_REGISTER_SPECIALIZATION(AddFloat)
inline constexpr AddFloat add_float;

} // namespace ac
