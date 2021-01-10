// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/common/select.hpp>
#include <actl/functional/scalar/comparison/less.hpp>
#include <limits>

namespace ac {

struct max_t : scalar_operation<max_t, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return eval(select(less(lhs, rhs), rhs, lhs));
    }
};
constexpr max_t max;

template <class T>
struct neutral_element<max_t, T> {
    static constexpr T value() {
        return std::numeric_limits<T>::lowest();
    }
};

}  // namespace ac
