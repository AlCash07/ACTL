// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct logical_not_f : scalar_operation<logical_not_f, 1>
{
    using category = logical_operation_tag;

    static constexpr bool eval_scalar(bool x)
    {
        return !x;
    }
};
inline constexpr logical_not_f logical_not;

} // namespace scalar

struct logical_not_f : operation<logical_not_f>
{
    using category = logical_operation_tag;

    static constexpr auto formula = scalar::logical_not;
};
inline constexpr logical_not_f logical_not;

template <class T, enable_operators<T> = 0>
constexpr auto operator!(T&& x)
{
    return logical_not(pass<T>(x));
}

} // namespace ac
