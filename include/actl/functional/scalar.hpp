/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>
#include <actl/numeric/math.hpp>

namespace ac::op {

/* Arithmetic operations */

struct arithmetic_operation_tag : scalar_operation_tag {};

template <class Derived, int Arity>
struct arithmetic_operation : scalar_operation<Derived, Arity> {
    using operation_tag = arithmetic_operation_tag;
};

struct Neg : arithmetic_operation<Neg, 1> {
    template <class T>
    static constexpr auto eval(const T& x) -> decltype(-x) {
        return -x;
    }
};

struct Sqr : arithmetic_operation<Sqr, 1> {};

struct Add : arithmetic_operation<Add, 2> {
    struct is_commutative;
    struct is_associative;

    template <class T, class U>
    static constexpr auto eval(const T& x, const U& y) -> decltype(x + y) {
        return x + y;
    }
};

struct Div : arithmetic_operation<Div, 2> {
    template <class T, class U>
    static constexpr auto eval(const T& x, const U& y) -> decltype(x / y) {
        return x / y;
    }
};

struct Mul : arithmetic_operation<Mul, 2> {
    struct is_commutative;
    struct is_associative;

    template <class T, class U>
    static constexpr auto eval(const T& x, const U& y) -> decltype(x * y) {
        return x * y;
    }
};

struct Sub : arithmetic_operation<Sub, 2> {
    template <class T, class U>
    static constexpr auto eval(const T& x, const U& y) -> decltype(x - y) {
        return x - y;
    }
};

inline constexpr Neg neg;
inline constexpr Sqr sqr;

inline constexpr Add add;
inline constexpr Div div;
inline constexpr Mul mul;
inline constexpr Sub sub;

template <class T>
inline constexpr auto perform(Sqr, const T& x) {
    return mul(x, x);
}

template <class T>
inline constexpr auto operator - (const T& x) {
    return neg(x);
}

template <class T, class U>
inline constexpr auto operator + (const T& lhs, const U& rhs) {
    return add(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator / (const T& lhs, const U& rhs) {
    return div(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator * (const T& lhs, const U& rhs) {
    return mul(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator - (const T& lhs, const U& rhs) {
    return sub(lhs, rhs);
}

/* Comparison operations */

struct comparison_operation_tag : scalar_operation_tag {};

template <class Derived, int Arity>
struct comparison_operation : scalar_operation<Derived, Arity> {
    using operation_tag = comparison_operation_tag;
};

struct Sgn : scalar_operation<Sgn, 1> {};

struct Cmp3Way : comparison_operation<Cmp3Way, 2> {};

struct Equal : comparison_operation<Equal, 2> {
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval(const T& x, const U& y) -> decltype(x == y) {
        return x == y;
    }
};

struct Less : comparison_operation<Less, 2> {
    template <class T, class U>
    static constexpr auto eval(const T& x, const U& y) -> decltype(x < y) {
        return x < y;
    }
};

struct Max : scalar_operation<Max, 2> {
    struct is_commutative;
    struct is_associative;
};

struct Min : scalar_operation<Min, 2> {
    struct is_commutative;
    struct is_associative;
};

inline constexpr Sgn sgn;

inline constexpr Cmp3Way cmp3way;
inline constexpr Equal equal;
inline constexpr Less less;

inline constexpr Max max;
inline constexpr Min min;

template <class T>
inline constexpr auto perform(Sgn, const T& x) {
    return cmp3way(x, 0);
}

template <class Policy, class T, class U>
inline constexpr int perform(const Policy& policy, Cmp3Way, const T& lhs, const U& rhs) {
    return (int)less(policy, rhs, lhs) - (int)less(policy, lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator == (const T& lhs, const U& rhs) {
    return equal(lhs, rhs);
}

template <class Policy, class T, class U>
inline constexpr decltype(auto) perform(const Policy& policy, Max, const T& lhs, const U& rhs) {
    return less(policy, lhs, rhs) ? rhs : lhs;
}

template <class Policy, class T, class U>
inline constexpr decltype(auto) perform(const Policy& policy, Min, const T& lhs, const U& rhs) {
    return less(policy, rhs, lhs) ? rhs : lhs;
}

template <class T, class U>
inline constexpr auto operator < (const T& lhs, const U& rhs) {
    return less(lhs, rhs);
}

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

template <class E, class T>
inline constexpr int perform(const absolute_error<E>& policy, Sgn, const T& x) {
    if (less(adl::abs(eval(policy, x)), policy.epsilon())) return 0;
    return x < 0 ? -1 : 1;
}

template <class Op, class E, class T, class U,
          enable_int_if<std::is_base_of_v<comparison_operation_tag, operation_tag_t<Op>>> = 0>
inline constexpr auto perform(const absolute_error<E>& policy, Op op, const T& lhs, const U& rhs) {
    return op(default_policy, sgn(policy, sub(lhs, rhs)), 0);
}

}  // namespace ac::op

namespace ac {

template <class Policy, class T, class U>
inline constexpr T& smax(const Policy& policy, T& x, const U& y) {
    return op::less(policy, x, y) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smax(T& x, const U& y) {
    return smax(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smin(const Policy& policy, T& x, const U& y) {
    return op::less(policy, y, x) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smin(T& x, const U& y) {
    return smin(default_policy, x, y);
}

}  // namespace ac
