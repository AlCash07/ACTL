// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct bit_and_f : scalar_operation<bit_and_f, 2>
{
    using category = bitwise_operation_tag;
    using argument_category = integral_tag;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs)
    {
        return lhs & rhs;
    }
};
inline constexpr bit_and_f bit_and;

} // namespace scalar

struct bit_and_f : operation<bit_and_f>
{
    using category = bitwise_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::bit_and;
};
inline constexpr bit_and_f bit_and;

template <class T>
struct identity_element<bit_and_f, T>
{
    static constexpr T value()
    {
        return T{-1};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator&(T&& lhs, U&& rhs)
{
    return bit_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator&=(T&& lhs, U&& rhs)
{
    return bit_and(inout{std::forward<T>(lhs)}, pass<U>(rhs));
}

} // namespace ac
