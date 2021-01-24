// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/comparison/equal.hpp>
#include <actl/operation/scalar/logical/logical_not.hpp>

namespace ac {

struct non_equal_t : scalar_operation<non_equal_t, 2> {
    using category = equality_operation_tag;
    using argument_category = scalar_tag;

    static constexpr bool is_commutative = true;

    static constexpr auto formula = !equal;
};
constexpr non_equal_t not_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator!=(T&& lhs, U&& rhs) {
    return not_equal(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
