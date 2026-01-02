// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/if_else.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <limits>

namespace ac {

struct Min : operation_base<Min> {
    using operation_category = comparison_operation;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return eval(if_else(less(r, l), r, l));
    }
};
inline constexpr Min min;

template<typename T>
struct identity_element<Min, T> {
    static constexpr T value() {
        return std::numeric_limits<T>::max();
    }
};

struct Max : operation_base<Max> {
    using operation_category = comparison_operation;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return eval(if_else(is_less(l, r), r, l));
    }
};
inline constexpr Max max;

template<typename T>
struct identity_element<Max, T> {
    static constexpr T value() {
        return std::numeric_limits<T>::lowest();
    }
};

} // namespace ac
