/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>

namespace ac::math {

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
        -> decltype(eval(policy, perform_policy(Op{}, policy, eval(policy, xs)...)), std::true_type{});
    
    static auto can_eval(...) -> std::false_type; 

    template <class Policy, class... Ts>
    static constexpr decltype(auto) evaluate(const Policy& policy, const Ts&... xs) {
        return eval(policy, perform_policy(Op{}, policy, eval(policy, xs)...));
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
template <class T>
inline constexpr Cast<T> cast;

struct Copy : scalar_operation<Copy> {
    template <class T>
    static constexpr auto perform(const T& x) {
        return x;
    }
};
inline constexpr Copy copy;

struct Ternary : scalar_operation<Ternary> {
    template <class B, class T, class U>
    static constexpr auto perform(const B& condition, const T& lhs, const U& rhs)
        -> remove_cvref_t<decltype(condition ? lhs : rhs)> {
        return condition ? lhs : rhs;
    }
};
inline constexpr Ternary ternary;

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
inline constexpr LogicalNot logical_not;

template <class T>
inline constexpr auto operator ! (T&& x) {
    return logical_not(pass<T>(x));
}

struct LogicalAnd : logical_operation<LogicalAnd> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs && rhs) {
        return lhs && rhs;
    }
};
inline constexpr LogicalAnd logical_and;

template <class T, class U>
inline constexpr auto operator && (T&& lhs, U&& rhs) {
    return logical_and(pass<T>(lhs), pass<U>(rhs));
}

struct LogicalOr : logical_operation<LogicalOr> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs || rhs) {
        return lhs || rhs;
    }
};
inline constexpr LogicalOr logical_or;

template <class T, class U>
inline constexpr auto operator || (T&& lhs, U&& rhs) {
    return logical_or(pass<T>(lhs), pass<U>(rhs));
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

struct Equal : comparison_operation<Equal> {
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs == rhs) {
        return lhs == rhs;
    }
};
inline constexpr Equal equal;

template <class T, class U>
inline constexpr auto operator == (T&& lhs, U&& rhs) {
    return equal(pass<T>(lhs), pass<U>(rhs));
}

struct Less : comparison_operation<Less> {
    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs < rhs) {
        return lhs < rhs;
    }
};
inline constexpr Less less;

template <class T, class U>
inline constexpr auto operator < (T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

struct Cmp3Way : comparison_operation<Cmp3Way> {
    // TODO: figure out a way to correctly handle rvalues here.
    template <class T, class U>
    static constexpr auto perform(const T& lhs, const U& rhs) {
        return cast<int>(less(rhs, lhs)) - cast<int>(less(lhs, rhs));
    }
};
inline constexpr Cmp3Way cmp3way;

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
inline constexpr Neg neg;

template <class T>
inline constexpr auto operator - (T&& x) {
    return neg(pass<T>(x));
}

struct Add : arithmetic_operation<Add> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs + rhs) {
        return lhs + rhs;
    }
};
inline constexpr Add add;

template <class T, class U>
inline constexpr auto operator + (T&& lhs, U&& rhs) {
    return add(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator += (T& lhs, const U& rhs) {
    return add(inplace(lhs), rhs);
}

struct Div : arithmetic_operation<Div> {
    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs / rhs) {
        return lhs / rhs;
    }
};
inline constexpr Div div;

template <class T, class U>
inline constexpr auto operator / (T&& lhs, U&& rhs) {
    return div(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator /= (T& lhs, const U& rhs) {
    return div(inplace(lhs), rhs);
}

struct Mul : arithmetic_operation<Mul> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs * rhs) {
        return lhs * rhs;
    }
};
inline constexpr Mul mul;

template <class T, class U>
inline constexpr auto operator * (T&& lhs, U&& rhs) {
    return mul(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator *= (T& lhs, const U& rhs) {
    return mul(inplace(lhs), rhs);
}

struct Sub : arithmetic_operation<Sub> {
    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs - rhs) {
        return lhs - rhs;
    }
};
inline constexpr Sub sub;

template <class T, class U>
inline constexpr auto operator - (T&& lhs, U&& rhs) {
    return sub(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator -= (T& lhs, const U& rhs) {
    return sub(inplace(lhs), rhs);
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
inline constexpr BitNot bit_not;

template <class T>
inline constexpr auto operator ~ (T&& x) {
    return bit_not(pass<T>(x));
}

struct BitAnd : bit_operation<BitAnd> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs & rhs) {
        return lhs & rhs;
    }
};
inline constexpr BitAnd bit_and;

template <class T, class U>
inline constexpr auto operator & (T&& lhs, U&& rhs) {
    return bit_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator &= (T& lhs, const U& rhs) {
    return bit_and(inplace(lhs), rhs);
}

struct BitOr : bit_operation<BitOr> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs | rhs) {
        return lhs | rhs;
    }
};
inline constexpr BitOr bit_or;

template <class T, class U>
inline constexpr auto operator | (T&& lhs, U&& rhs) {
    return bit_or(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator |= (T& lhs, const U& rhs) {
    return bit_or(inplace(lhs), rhs);
}

struct BitXor : bit_operation<BitXor> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr auto perform(const T& lhs, const T& rhs) -> decltype(lhs ^ rhs) {
        return lhs ^ rhs;
    }
};
inline constexpr BitXor bit_xor;

template <class T, class U>
inline constexpr auto operator ^ (T&& lhs, U&& rhs) {
    return bit_xor(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator ^= (T& lhs, const U& rhs) {
    return bit_xor(inplace(lhs), rhs);
}

/* Derived operations */

struct Max : scalar_operation<Max> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto perform(const T& lhs, const U& rhs) {
        return ternary(less(lhs, rhs), rhs, lhs);
    }
};
inline constexpr Max max;

struct Min : scalar_operation<Min> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto perform(const T& lhs, const U& rhs) {
        return ternary(less(rhs, lhs), rhs, lhs);
    }
};
inline constexpr Min min;

struct Sgn : scalar_operation<Sgn> {
    template <class T>
    static constexpr auto perform(const T& x) {
        return cmp3way(x, T{0});
    }
};
inline constexpr Sgn sgn;

struct Sqr : scalar_operation<Sqr> {
    template <class T>
    static constexpr auto perform(const T& x) {
        return mul(x, x);
    }
};
inline constexpr Sqr sqr;

}  // namespace ac::math
