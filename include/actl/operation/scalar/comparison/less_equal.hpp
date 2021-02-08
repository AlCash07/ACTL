// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/comparison/greater.hpp>
#include <actl/operation/scalar/logical/logical_not.hpp>

namespace ac {

struct less_equal_f : scalar_operation<less_equal_f, 2> {
    using category = ordering_operation_tag;
    using argument_category = scalar_tag;

    static constexpr auto formula = !greater;
};
inline constexpr less_equal_f less_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator<=(T&& lhs, U&& rhs) {
    return less_equal(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
