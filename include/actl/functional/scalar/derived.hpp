// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/common.hpp>
#include <actl/functional/scalar/comparison.hpp>

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

struct min_t : scalar_operation<min_t, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return eval(select(less(rhs, lhs), rhs, lhs));
    }
};
constexpr min_t min;

struct sgn_t : scalar_operation<sgn_t, 1, scalar_tag> {
    static constexpr auto formula = cmp3way(x_, zero);
};
constexpr sgn_t sgn;

struct sqr_t : scalar_operation<sqr_t, 1, arithmetic_tag> {
    static constexpr auto formula = mul(x_, x_);
};
constexpr sqr_t sqr;

}  // namespace ac
