// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>
#include <cmath>
#include <cstdlib>

namespace ac {

struct abs_t : scalar_operation<abs_t, 1, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        if constexpr (std::is_unsigned_v<T>) {
            return x;
        } else {
            using std::abs;
            return abs(x);
        }
    }
};
constexpr abs_t abs;

#define MATH_OP1(name, op)                               \
    struct name : scalar_operation<name, 1, float_tag> { \
        template <class T>                               \
        static constexpr auto eval_scalar(T x) {         \
            using std::op;                               \
            return op(x);                                \
        }                                                \
    };                                                   \
    constexpr name op;

#define MATH_OP2(name, op)                                \
    struct name : scalar_operation<name, 2, float_tag> {  \
        template <class T, class U>                       \
        static constexpr auto eval_scalar(T lhs, U rhs) { \
            using std::op;                                \
            return op(lhs, rhs);                          \
        }                                                 \
    };                                                    \
    constexpr name op;

MATH_OP1(cos_t, cos)
MATH_OP1(sin_t, sin)
MATH_OP1(sqrt_t, sqrt)

MATH_OP2(atan2_t, atan2)

#undef MATH_OP1
#undef MATH_OP2

}  // namespace ac
