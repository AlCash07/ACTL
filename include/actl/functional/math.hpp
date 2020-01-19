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

namespace ac::op {

struct math_operation_tag : scalar_operation_tag {};

template <class Derived, int Arity>
struct math_operation : scalar_operation<Derived, Arity> {
    using operation_tag = math_operation_tag;
};

struct Abs : math_operation<Abs, 1> {
    template <class T>
    static constexpr T eval(const T& x) {
        if constexpr (std::is_unsigned_v<T>) {
            return x;
        } else {
            return std::abs(x);
        }
    }
};

inline constexpr Abs abs;

#define MATH_OP1(name, op)                                               \
    struct name : math_operation<name, 1> {                              \
        template <class T>                                               \
        static constexpr auto eval(const T& x) -> decltype(std::op(x)) { \
            return std::op(x);                                           \
        }                                                                \
    };                                                                   \
    inline constexpr name op;

#define MATH_OP2(name, op)                                                              \
    struct name : math_operation<name, 2> {                                             \
        template <class T>                                                              \
        static constexpr auto eval(const T& x, const T& y) -> decltype(std::op(x, y)) { \
            return std::op(x, y);                                                       \
        }                                                                               \
    };                                                                                  \
    inline constexpr name op;

MATH_OP1(Cos, cos)
MATH_OP1(Sin, sin)
MATH_OP1(Sqrt, sqrt)

MATH_OP2(Atan2, atan2)

#undef MATH_OP1
#undef MATH_OP2

/**
 * Comparable square root with deferred call to sqrt.
 */
template <class T>
class square_root {
public:
    constexpr explicit square_root(const T& value = {}) : sqr_{value} {}

    operator decltype(eval(sqrt(std::declval<T>())))() const { return eval(sqrt(sqr_)); }

    friend constexpr const T& eval(policy, Sqr, const square_root& x) { return x.sqr_; }

    template <class Op, enable_int_if<is_comparison_operation_v<Op>> = 0>
    friend constexpr auto perform(Op op, const square_root& lhs, const square_root& rhs) {
        return op(lhs.sqr_, rhs.sqr_);
    }

private:
    T sqr_;
};

struct defer_sqrt : virtual policy {};

template <class T>
inline auto perform(defer_sqrt, Sqrt, const T& x) {
    return square_root{x};
}

}  // namespace ac::op
