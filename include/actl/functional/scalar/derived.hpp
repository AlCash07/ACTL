// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/common.hpp>
#include <actl/functional/scalar/comparison.hpp>

namespace ac::math {

struct Max : scalar_operation<Max, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return eval(select(less(lhs, rhs), rhs, lhs));
    }
};
constexpr Max max;

struct Min : scalar_operation<Min, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return eval(select(less(rhs, lhs), rhs, lhs));
    }
};
constexpr Min min;

struct Sgn : scalar_operation<Sgn, 1, scalar_tag> {
    static constexpr auto formula = cmp3way(x_, zero);
};
constexpr Sgn sgn;

struct Sqr : scalar_operation<Sqr, 1, arithmetic_tag> {
    static constexpr auto formula = mul(x_, x_);
};
constexpr Sqr sqr;

}  // namespace ac::math
