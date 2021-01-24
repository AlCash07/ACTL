// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct less_t : scalar_operation<less_t, 2> {
    using category = ordering_operation_tag;
    using argument_category = scalar_tag;

    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs < rhs;
    }
};
constexpr less_t less;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator<(T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
