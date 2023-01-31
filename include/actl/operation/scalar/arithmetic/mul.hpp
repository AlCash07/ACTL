// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct mul_f : scalar_operation<mul_f, 2>
{
    using operation_category = multiplicative_operation_tag;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs)
    {
        return lhs * rhs;
    }
};
inline constexpr mul_f mul;

} // namespace scalar

struct mul_f : operation<mul_f>
{
    using operation_category = multiplicative_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::mul;
};
inline constexpr mul_f mul;

template <class T>
struct identity_element<mul_f, T>
{
    static constexpr T value()
    {
        return T{1};
    }
};

template <class T, class U>
    requires EnableOperators<T, U>
constexpr auto operator*(T&& lhs, U&& rhs)
{
    return mul(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
    requires EnableOperators<T, U>
constexpr decltype(auto) operator*=(T&& lhs, U&& rhs)
{
    return mul(inout{std::forward<T>(lhs)}, pass<U>(rhs));
}

} // namespace ac
