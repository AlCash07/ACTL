// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/comparison/less.hpp>
#include <actl/operation/scalar/logical/logical_not.hpp>

namespace ac {

struct greater_equal_f : operation<greater_equal_f>
{
    using category = ordering_operation_tag;

    static constexpr auto formula = !less;
};
inline constexpr greater_equal_f greater_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator>=(T&& lhs, U&& rhs)
{
    return greater_equal(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
