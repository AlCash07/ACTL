// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct bit_not_f : scalar_operation<bit_not_f, 1>
{
    using operation_category = bitwise_operation_tag;

    template <class T>
    static constexpr auto eval_scalar(T x)
    {
        return ~x;
    }
};
inline constexpr bit_not_f bit_not;

} // namespace scalar

struct bit_not_f : operation<bit_not_f>
{
    using operation_category = bitwise_operation_tag;

    static constexpr auto formula = scalar::bit_not;
};
inline constexpr bit_not_f bit_not;

template <class T>
    requires EnableOperators<T>
constexpr auto operator~(T&& x)
{
    return bit_not(pass<T>(x));
}

} // namespace ac
