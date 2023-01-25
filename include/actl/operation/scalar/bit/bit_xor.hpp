// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct bit_xor_f : scalar_operation<bit_xor_f, 2>
{
    using category = bitwise_operation_tag;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs)
    {
        return lhs ^ rhs;
    }
};
inline constexpr bit_xor_f bit_xor;

} // namespace scalar

struct bit_xor_f : operation<bit_xor_f>
{
    using category = bitwise_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::bit_xor;
};
inline constexpr bit_xor_f bit_xor;

template <class T>
struct identity_element<bit_xor_f, T>
{
    static constexpr T value()
    {
        return T{0};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator^(T&& lhs, U&& rhs)
{
    return bit_xor(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator^=(T&& lhs, U&& rhs)
{
    return bit_xor(inout{std::forward<T>(lhs)}, pass<U>(rhs));
}

} // namespace ac
