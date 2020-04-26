/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>

namespace ac::op {

struct scalar_operation_tag : base_operation_tag {};

template <class T>
inline constexpr bool is_scalar_operation_v =
    std::is_base_of_v<scalar_operation_tag, operation_tag_t<T>>;

template <class Derived>
struct scalar_operation : operation<Derived> {
    using operation_tag = scalar_operation_tag;
};

template <class Tag, class Op>
struct calculator<
    Tag, Op, std::enable_if_t<std::is_base_of_v<scalar_tag, Tag> && is_scalar_operation_v<Op>>> {
    template <class Policy, class... Ts>
    static auto can_eval(const Policy& policy, const Ts&... xs)
        -> decltype(eval(policy, perform(policy, Op{}, eval(policy, xs)...)), std::true_type{});
    
    static auto can_eval(...) -> std::false_type; 

    template <class Policy, class... Ts>
    static constexpr decltype(auto) evaluate(const Policy& policy, const Ts&... xs) {
        return eval(policy, perform(policy, Op{}, eval(policy, xs)...));
    }

    template <class T, class... Ts>
    static auto& get_dst(T&, Ts&... xs) {
        return get_dst(xs...);
    }

    template <bool In, class T, class... Ts>
    static T& get_dst(out<In, T>& x, Ts&... xs) {
        return x.x;
    }

    template <class Policy, class... Ts>
    static constexpr auto& assign(const Policy& policy, Ts&... xs) {
        return get_dst(xs...) = evaluate(policy, xs...);
    }

    template <class Policy, class T, class... Ts>
    static constexpr T& assign(const Policy& policy, out<false, T>& dst, Ts&... xs) {
        return dst.x = evaluate(policy, xs...);
    }
};

template <class To>
struct Cast : scalar_operation<Cast<To>> {
    template <class T>
    static constexpr auto perform(const T& x) -> decltype(static_cast<To>(x)) {
        return static_cast<To>(x);
    }
};

struct Copy : scalar_operation<Copy> {
    template <class T>
    static constexpr auto perform(const T& x) {
        return x;
    }
};

struct Ternary : scalar_operation<Ternary> {
    template <class B, class T, class U>
    static constexpr auto perform(const B& condition, const T& lhs, const U& rhs)
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

template <class Derived>
struct arithmetic_operation : scalar_operation<Derived> {
    using operation_tag = arithmetic_operation_tag;
};

struct Neg : arithmetic_operation<Neg> {
    template <class T>
    static constexpr auto perform(const T& x) -> decltype(-x) {
        return -x;
    }
};

struct Add : arithmetic_operation<Add> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x + y) {
        return x + y;
    }
};

struct Div : arithmetic_operation<Div> {
    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x / y) {
        return x / y;
    }
};

struct Mul : arithmetic_operation<Mul> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x * y) {
        return x * y;
    }
};

struct Sub : arithmetic_operation<Sub> {
    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x - y) {
        return x - y;
    }
};

inline constexpr Neg neg;
inline constexpr Add add;
inline constexpr Div div;
inline constexpr Mul mul;
inline constexpr Sub sub;

template <class T, int_if_adl<T> = 0>
inline constexpr auto operator - (T&& x) {
    return neg(pass<T>(x));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator + (T&& lhs, U&& rhs) {
    return add(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator / (T&& lhs, U&& rhs) {
    return div(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator * (T&& lhs, U&& rhs) {
    return mul(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator - (T&& lhs, U&& rhs) {
    return sub(pass<T>(lhs), pass<U>(rhs));
}

/* Comparison operations */

struct comparison_operation_tag : scalar_operation_tag {};

template <class T>
inline constexpr bool is_comparison_operation_v =
    std::is_base_of_v<comparison_operation_tag, operation_tag_t<T>>;

template <class Derived>
struct comparison_operation : scalar_operation<Derived> {
    using operation_tag = comparison_operation_tag;
};

struct Cmp3Way : comparison_operation<Cmp3Way> {};

struct Equal : comparison_operation<Equal> {
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x == y) {
        return x == y;
    }
};

struct Less : comparison_operation<Less> {
    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x < y) {
        return x < y;
    }
};

inline constexpr Cmp3Way cmp3way;
inline constexpr Equal equal;
inline constexpr Less less;

// TODO: figure out a way to correctly handle rvalues here.
template <class T, class U>
inline constexpr auto perform(Cmp3Way, const T& lhs, const U& rhs) {
    return cast<int>(less(rhs, lhs)) - cast<int>(less(lhs, rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator == (T&& lhs, U&& rhs) {
    return equal(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator < (T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

/* Logical operations */

struct logical_operation_tag : scalar_operation_tag {};

template <class Derived>
struct logical_operation : scalar_operation<Derived> {
    using operation_tag = logical_operation_tag;
};

struct LogicalNot : logical_operation<LogicalNot> {
    template <class T>
    static constexpr auto perform(const T& x) -> decltype(!x) {
        return !x;
    }
};

struct LogicalAnd : logical_operation<LogicalAnd> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x && y) {
        return x && y;
    }
};

struct LogicalOr : logical_operation<LogicalOr> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x || y) {
        return x || y;
    }
};

inline constexpr LogicalNot logical_not;
inline constexpr LogicalAnd logical_and;
inline constexpr LogicalOr logical_or;

template <class T, int_if_adl<T> = 0>
inline constexpr auto operator ! (T&& x) {
    return logical_not(pass<T>(x));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator && (T&& lhs, U&& rhs) {
    return logical_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator || (const T& lhs, const U& rhs) {
    return logical_or(pass<T>(lhs), pass<U>(rhs));
}

/* Bit operations */

struct bit_operation_tag : scalar_operation_tag {};

template <class Derived>
struct bit_operation : scalar_operation<Derived> {
    using operation_tag = bit_operation_tag;
};

struct BitNot : bit_operation<BitNot> {
    template <class T>
    static constexpr auto perform(const T& x) -> decltype(~x) {
        return ~x;
    }
};

struct BitAnd : bit_operation<BitAnd> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x & y) {
        return x & y;
    }
};

struct BitOr : bit_operation<BitOr> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x | y) {
        return x | y;
    }
};

struct BitXor : bit_operation<BitXor> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& x, const T& y) -> decltype(x ^ y) {
        return x ^ y;
    }
};

inline constexpr BitNot bit_not;
inline constexpr BitAnd bit_and;
inline constexpr BitOr bit_or;
inline constexpr BitXor bit_xor;

template <class T, int_if_adl<T> = 0>
inline constexpr auto operator ~ (T&& x) {
    return bit_not(pass<T>(x));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator & (T&& lhs, U&& rhs) {
    return bit_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator | (T&& lhs, U&& rhs) {
    return bit_or(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, int_if_adl<T, U> = 0>
inline constexpr auto operator ^ (T&& lhs, U&& rhs) {
    return bit_xor(pass<T>(lhs), pass<U>(rhs));
}

/* Derived operations */

struct Max : scalar_operation<Max> {
    struct is_associative;
    struct is_commutative;
};

struct Min : scalar_operation<Min> {
    struct is_associative;
    struct is_commutative;
};

struct Sgn : scalar_operation<Sgn> {};

struct Sqr : scalar_operation<Sqr> {};

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
