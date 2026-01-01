// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Add : operation_base<Add> {
    // TODO: uncomment this after fixing tests.
    // using operation_category = additive_operation;
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

struct AddIntegers : operation_base<AddIntegers> {
    using operation_category = additive_operation;

    using parent = Add;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement = std::integral<L> && std::integral<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
AC_REGISTER_OVERLOAD(AddIntegers)
inline constexpr AddIntegers add_integers;

struct AddFloats : operation_base<AddFloats> {
    using operation_category = additive_operation;

    using parent = Add;

    // floating point addition is neither associative nor commutative

    template<typename L, typename R>
    static constexpr bool requirement =
        std::floating_point<L> && std::floating_point<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
AC_REGISTER_OVERLOAD(AddFloats)
inline constexpr AddFloats add_floats;

} // namespace ac
