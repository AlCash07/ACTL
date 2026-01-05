// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Multiply : operation_base<Multiply> {
    using operation_category = multiplicative_operation;
};
inline constexpr Multiply multiply;

template<typename T>
struct identity_element<Multiply, T> {
    static constexpr T value() {
        return T{1};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator*(L&& l, R&& r) {
    return multiply(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator*=(L&& l, R&& r) {
    return multiply(inout{std::forward<L>(l)}, std::forward<R>(r));
}

struct MultiplyInteger : operation_base<MultiplyInteger> {
    using parent = Multiply;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement = std::integral<L> && std::integral<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l * r;
    }
};
AC_REGISTER_OVERLOAD(MultiplyInteger)
inline constexpr MultiplyInteger miltiply_integer;

struct MultiplyFloat : operation_base<MultiplyFloat> {
    using parent = Multiply;

    // floating point multiplication is neither associative nor commutative

    template<typename L, typename R>
    static constexpr bool requirement =
        std::floating_point<L> && std::floating_point<R>;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l * r;
    }
};
AC_REGISTER_OVERLOAD(MultiplyFloat)
inline constexpr MultiplyFloat miltiply_float;

} // namespace ac
