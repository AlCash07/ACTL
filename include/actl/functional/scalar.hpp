/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>

namespace ac::op {

template <class To>
struct Cast : scalar_operation<Cast<To>, 1> {
    template <class T>
    static constexpr auto eval(const T& x) -> decltype(static_cast<To>(x)) {
        return static_cast<To>(x);
    }
};

struct Copy : scalar_operation<Copy, 1> {
    template <class T>
    static constexpr auto& eval(const T& x) {
        return x;
    }
};

struct Ternary : scalar_operation<Ternary, 3> {
    template <class B, class T, class U>
    static constexpr auto eval(const B& condition, const T& lhs, const U& rhs)
        -> decltype(condition ? lhs : rhs) {
        return condition ? lhs : rhs;
    }
};

template <class T>
inline constexpr Cast<T> cast;
inline constexpr Copy copy;
inline constexpr Ternary ternary;

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

struct Add : arithmetic_operation<Add, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x + y) {
        return x + y;
    }
};

struct Div : arithmetic_operation<Div, 2> {
    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x / y) {
        return x / y;
    }
};

struct Mul : arithmetic_operation<Mul, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x * y) {
        return x * y;
    }
};

struct Sub : arithmetic_operation<Sub, 2> {
    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x - y) {
        return x - y;
    }
};

inline constexpr Neg neg;
inline constexpr Add add;
inline constexpr Div div;
inline constexpr Mul mul;
inline constexpr Sub sub;

template <class T, enable_adl<T> = 0>
inline constexpr auto operator - (const T& x) {
    return neg(x);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator + (const T& lhs, const U& rhs) {
    return add(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator / (const T& lhs, const U& rhs) {
    return div(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator * (const T& lhs, const U& rhs) {
    return mul(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator - (const T& lhs, const U& rhs) {
    return sub(lhs, rhs);
}

/* Comparison operations */

struct comparison_operation_tag : scalar_operation_tag {};

template <class T>
inline constexpr bool is_comparison_operation_v =
    std::is_base_of_v<comparison_operation_tag, operation_tag_t<T>>;

template <class Derived, int Arity>
struct comparison_operation : scalar_operation<Derived, Arity> {
    using operation_tag = comparison_operation_tag;
};

struct Cmp3Way : comparison_operation<Cmp3Way, 2> {};

struct Equal : comparison_operation<Equal, 2> {
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x == y) {
        return x == y;
    }
};

struct Less : comparison_operation<Less, 2> {
    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x < y) {
        return x < y;
    }
};

inline constexpr Cmp3Way cmp3way;
inline constexpr Equal equal;
inline constexpr Less less;

template <class T, class U>
inline constexpr auto perform(Cmp3Way, const T& lhs, const U& rhs) {
    return cast<int>(less(rhs, lhs)) - cast<int>(less(lhs, rhs));
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator == (const T& lhs, const U& rhs) {
    return equal(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator < (const T& lhs, const U& rhs) {
    return less(lhs, rhs);
}

/* Logical operations */

struct logical_operation_tag : scalar_operation_tag {};

template <class Derived, int Arity>
struct logical_operation : scalar_operation<Derived, Arity> {
    using operation_tag = logical_operation_tag;
};

struct LogicalNot : logical_operation<LogicalNot, 1> {
    template <class T>
    static constexpr auto eval(const T& x) -> decltype(!x) {
        return !x;
    }
};

struct LogicalAnd : logical_operation<LogicalAnd, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x && y) {
        return x && y;
    }
};

struct LogicalOr : logical_operation<LogicalOr, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x || y) {
        return x || y;
    }
};

inline constexpr LogicalNot logical_not;
inline constexpr LogicalAnd logical_and;
inline constexpr LogicalOr logical_or;

template <class T, enable_adl<T> = 0>
inline constexpr auto operator ! (const T& x) {
    return logical_not(x);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator && (const T& lhs, const U& rhs) {
    return logical_and(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator || (const T& lhs, const U& rhs) {
    return logical_or(lhs, rhs);
}

/* Bit operations */

struct bit_operation_tag : scalar_operation_tag {};

template <class Derived, int Arity>
struct bit_operation : scalar_operation<Derived, Arity> {
    using operation_tag = bit_operation_tag;
};

struct BitNot : bit_operation<BitNot, 1> {
    template <class T>
    static constexpr auto eval(const T& x) -> decltype(~x) {
        return ~x;
    }
};

struct BitAnd : bit_operation<BitAnd, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x & y) {
        return x & y;
    }
};

struct BitOr : bit_operation<BitOr, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x | y) {
        return x | y;
    }
};

struct BitXor : bit_operation<BitXor, 2> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto eval(const T& x, const T& y) -> decltype(x ^ y) {
        return x ^ y;
    }
};

inline constexpr BitNot bit_not;
inline constexpr BitAnd bit_and;
inline constexpr BitOr bit_or;
inline constexpr BitXor bit_xor;

template <class T, enable_adl<T> = 0>
inline constexpr auto operator ~ (const T& x) {
    return bit_not(x);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator & (const T& lhs, const U& rhs) {
    return bit_and(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator | (const T& lhs, const U& rhs) {
    return bit_or(lhs, rhs);
}

template <class T, class U, enable_adl<T, U> = 0>
inline constexpr auto operator ^ (const T& lhs, const U& rhs) {
    return bit_xor(lhs, rhs);
}

/* Derived operations */

struct Max : scalar_operation<Max, 2> {
    struct is_associative;
    struct is_commutative;
};

struct Min : scalar_operation<Min, 2> {
    struct is_associative;
    struct is_commutative;
};

struct Sgn : scalar_operation<Sgn, 1> {};

struct Sqr : scalar_operation<Sqr, 1> {};

template <class T, class U>
inline constexpr auto perform(Max, const T& lhs, const U& rhs) {
    return ternary(less(lhs, rhs), rhs, lhs);
}

template <class T, class U>
inline constexpr auto perform(Min, const T& lhs, const U& rhs) {
    return ternary(less(rhs, lhs), rhs, lhs);
}

template <class T>
inline constexpr auto perform(Sgn, const T& x) {
    return cmp3way(x, T{0});
}

template <class T>
inline constexpr auto perform(Sqr, const T& x) {
    return mul(x, x);
}

inline constexpr Max max;
inline constexpr Min min;
inline constexpr Sgn sgn;
inline constexpr Sqr sqr;

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
