/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>
#include <cmath>
#include <cstdlib>

namespace ac::op {

struct math_operation_tag : scalar_operation_tag {};

template <class Derived, int Arity>
struct math_operation : scalar_operation<Derived, Arity> {
    using operation_tag = math_operation_tag;
};

#define MATH_OP1(name, op)                                                  \
    struct name : math_operation<name, 1> {                                 \
        template <class T>                                                  \
        static constexpr auto perform(const T& x) -> decltype(std::op(x)) { \
            return std::op(x);                                              \
        }                                                                   \
    };                                                                      \
    inline constexpr name op;

#define MATH_OP2(name, op)                                                                 \
    struct name : math_operation<name, 2> {                                                \
        template <class T>                                                                 \
        static constexpr auto perform(const T& x, const T& y) -> decltype(std::op(x, y)) { \
            return std::op(x, y);                                                          \
        }                                                                                  \
    };                                                                                     \
    inline constexpr name op;

MATH_OP1(Abs, abs)
MATH_OP1(Cos, cos)
MATH_OP1(Sin, sin)
MATH_OP1(Sqrt, sqrt)

MATH_OP2(Atan2, atan2)

#undef MATH_OP1
#undef MATH_OP2

template <class T, enable_int_if<std::is_unsigned_v<T>> = 0>
inline constexpr const T& perform(Abs, const T& x) {
    return x;
}

}  // namespace ac::op
