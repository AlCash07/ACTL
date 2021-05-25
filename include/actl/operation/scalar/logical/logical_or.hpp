// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct logical_or_f : scalar_operation<logical_or_f, 2>
{
    using category = logical_operation_tag;
    using argument_category = boolean_tag;

    static constexpr bool eval_scalar(bool lhs, bool rhs)
    {
        return lhs || rhs;
    }
};
inline constexpr logical_or_f logical_or;

} // namespace scalar

struct logical_or_f : operation<logical_or_f>
{
    using category = logical_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::logical_or;
};
inline constexpr logical_or_f logical_or;

template <class T>
struct identity_element<logical_or_f, T>
{
    static constexpr T value()
    {
        return T{false};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator||(T&& lhs, U&& rhs)
{
    return logical_or(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
