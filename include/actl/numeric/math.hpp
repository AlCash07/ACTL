// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>
#include <cmath>
#include <cstdlib>

namespace ac {

struct math_operation_tag
{
    using base = scalar_operation_tag;
};

struct abs_t : scalar_operation<abs_t, 1>
{
    using category = math_operation_tag;
    using argument_category = arithmetic_tag;

    template <class T>
    static constexpr T eval_scalar(T x)
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            return x;
        }
        else
        {
            using std::abs;
            return abs(x);
        }
    }
};
inline constexpr abs_t abs;

template <class Op, size_t Arity>
struct math_operation : scalar_operation<Op, Arity>
{
    using category = math_operation_tag;
    using argument_category = floating_point_tag;
};

#define MATH_OP1(name, op)                     \
    struct name : math_operation<name, 1>      \
    {                                          \
        template <class T>                     \
        static constexpr auto eval_scalar(T x) \
        {                                      \
            using std::op;                     \
            return op(x);                      \
        }                                      \
    };                                         \
    constexpr name op;

#define MATH_OP2(name, op)                              \
    struct name : math_operation<name, 2>               \
    {                                                   \
        template <class T, class U>                     \
        static constexpr auto eval_scalar(T lhs, U rhs) \
        {                                               \
            using std::op;                              \
            return op(lhs, rhs);                        \
        }                                               \
    };                                                  \
    constexpr name op;

MATH_OP1(cos_t, cos)
MATH_OP1(sin_t, sin)
MATH_OP1(sqrt_t, sqrt)

MATH_OP2(atan2_t, atan2)

#undef MATH_OP1
#undef MATH_OP2

} // namespace ac
