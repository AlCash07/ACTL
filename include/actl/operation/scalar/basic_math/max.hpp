// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/common/select.hpp>
#include <actl/operation/scalar/comparison/less.hpp>
#include <limits>

namespace ac {

struct max_f : scalar_operation<max_f, 2> {
    using category = scalar_operation_tag;
    using argument_category = scalar_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return eval(select(less(lhs, rhs), rhs, lhs));
    }
};
inline constexpr max_f max;

template <class T>
struct identity_element<max_f, T> {
    static constexpr T value() {
        return std::numeric_limits<T>::lowest();
    }
};

} // namespace ac
