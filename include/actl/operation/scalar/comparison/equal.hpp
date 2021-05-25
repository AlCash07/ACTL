// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct equal_f : scalar_operation<equal_f, 2>
{
    using category = equality_operation_tag;
    using argument_category = scalar_tag;

    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs)
    {
        return lhs == rhs;
    }
};
inline constexpr equal_f equal;

} // namespace scalar

struct equal_f : operation<equal_f>
{
    using category = equality_operation_tag;

    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::equal;
};
inline constexpr equal_f equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator==(T&& lhs, U&& rhs)
{
    return equal(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
