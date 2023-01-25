// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct neg_f : scalar_operation<neg_f, 1>
{
    using category = additive_operation_tag;

    template <class T>
    static constexpr auto eval_scalar(T x)
    {
        return -x;
    }
};
inline constexpr neg_f neg;

} // namespace scalar

struct neg_f : operation<neg_f>
{
    using category = additive_operation_tag;

    static constexpr auto formula = scalar::neg;
};
inline constexpr neg_f neg;

template <class T, enable_operators<T> = 0>
constexpr auto operator-(T&& x)
{
    return neg(pass<T>(x));
}

} // namespace ac
