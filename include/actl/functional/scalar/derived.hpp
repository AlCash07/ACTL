/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/common.hpp>
#include <actl/functional/scalar/comparison.hpp>

namespace ac::math {

struct Max : scalar_operation<Max, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
        return select(less(lhs, rhs), rhs, lhs);
    }
};
inline constexpr Max max;

struct Min : scalar_operation<Min, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
        return select(less(rhs, lhs), rhs, lhs);
    }
};
inline constexpr Min min;

struct Sgn : scalar_operation<Sgn, 1, scalar_tag> {
    template <class T>
    static constexpr int eval_scalar(T x) {
        return cmp3way(x, T{0});
    }
};
inline constexpr Sgn sgn;

struct Sqr : scalar_operation<Sqr, 1, arithmetic_tag> {
    static constexpr auto formula = mul(x_, x_);
};
inline constexpr Sqr sqr;

}  // namespace ac::math
