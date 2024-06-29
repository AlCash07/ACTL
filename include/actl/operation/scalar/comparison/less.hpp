// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct less_f : scalar_operation<less_f, 2> {
    using operation_category = ordering_operation_tag;

    template<class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs < rhs;
    }
};
inline constexpr less_f less;

} // namespace scalar

struct less_f : operation<less_f> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = scalar::less;
};
inline constexpr less_f less;

template<class T, class U>
    requires EnableOperators<T, U>
constexpr auto operator<(T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
