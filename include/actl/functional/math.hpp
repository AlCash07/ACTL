/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar.hpp>
#include <cmath>
#include <cstdlib>

namespace ac::math {

struct Abs : operation<Abs, 1, arithmetic_tag> {
    template <class T>
    static constexpr T perform(T x) {
        if constexpr (std::is_unsigned_v<T>) {
            return x;
        } else {
            return std::abs(x);
        }
    }
};
inline constexpr Abs abs;

#define MATH_OP1(name, op)                                           \
    struct name : operation<name, 1, float_tag> {                    \
        template <class T>                                           \
        static constexpr auto perform(T x) -> decltype(std::op(x)) { \
            return std::op(x);                                       \
        }                                                            \
    };                                                               \
    inline constexpr name op;

#define MATH_OP2(name, op)                                                           \
    struct name : operation<name, 2, float_tag> {                                    \
        template <class T>                                                           \
        static constexpr auto perform(T lhs, T rhs) -> decltype(std::op(lhs, rhs)) { \
            return std::op(lhs, rhs);                                                \
        }                                                                            \
    };                                                                               \
    inline constexpr name op;

MATH_OP1(Cos, cos)
MATH_OP1(Sin, sin)
MATH_OP1(Sqrt, sqrt)

MATH_OP2(Atan2, atan2)

#undef MATH_OP1
#undef MATH_OP2

}  // namespace ac::math
