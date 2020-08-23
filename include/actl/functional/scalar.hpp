/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/operation.hpp>
#include <actl/util/none.hpp>

namespace ac::math {

struct arithmetic_tag : scalar_tag     { using base = scalar_tag; };
struct float_tag      : arithmetic_tag { using base = arithmetic_tag; };
struct integral_tag   : arithmetic_tag { using base = arithmetic_tag; };
struct boolean_tag    : integral_tag   { using base = integral_tag; };

template <class T>
struct category_impl<T, std::enable_if_t<std::is_arithmetic_v<T> && !std::is_integral_v<T> &&
                                         !std::is_floating_point_v<T>>> {
    using type = arithmetic_tag;
};

template <class T>
struct category_impl<T, std::enable_if_t<std::is_floating_point_v<T>>> {
    using type = float_tag;
};

template <class T>
struct category_impl<T, std::enable_if_t<std::is_integral_v<T>>> {
    using type = integral_tag;
};

template <>
struct category_impl<bool> {
    using type = boolean_tag;
};

template <class Op, index Arity, class ArgumentsTag>
struct scalar_operation : operation<Op> {
    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const {
        static_assert((... && std::is_base_of_v<ArgumentsTag, category_t<Ts>>));
        return this->derived().eval_scalar(eval(xs)...);
    }

    template <class T, class... Ts>
    constexpr void assign(T& dst, const Ts&... xs) const {
        dst = evaluate(xs...);
    }
};

template <class To>
struct Cast : scalar_operation<Cast<To>, 1, arithmetic_tag> {
    template <class T>
    static constexpr To eval_scalar(T x) {
        return static_cast<To>(x);
    }
};
template <class T>
inline constexpr Cast<T> cast;

struct Copy : scalar_operation<Copy, 1, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return x;
    }
};
inline constexpr Copy copy;

template <bool In, class T>
template <class U>
inline out<In, T>& out<In, T>::operator=(const U& y) {
    static_assert(!In, "use output instead of inplace argument for assignment");
    copy(*this, y);
    return *this;
}

struct Ternary : scalar_operation<Ternary, 3, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(bool condition, T lhs, T rhs) {
        return condition ? lhs : rhs;
    }
};
inline constexpr Ternary ternary;

/* Logical operations */

struct LogicalNot : scalar_operation<LogicalNot, 1, boolean_tag> {
    static constexpr bool eval_scalar(bool x) { return !x; }
};
inline constexpr LogicalNot logical_not;

template <class T>
inline constexpr auto operator ! (T&& x) {
    return logical_not(pass<T>(x));
}

struct LogicalAnd : scalar_operation<LogicalAnd, 2, boolean_tag> {
    struct is_associative;
    struct is_commutative;

    static constexpr bool eval_scalar(bool lhs, bool rhs) { return lhs && rhs; }
};
inline constexpr LogicalAnd logical_and;

template <class T, class U>
inline constexpr auto operator && (T&& lhs, U&& rhs) {
    return logical_and(pass<T>(lhs), pass<U>(rhs));
}

struct LogicalOr : scalar_operation<LogicalOr, 2, boolean_tag> {
    struct is_associative;
    struct is_commutative;

    static constexpr bool eval_scalar(bool lhs, bool rhs) { return lhs || rhs; }
};
inline constexpr LogicalOr logical_or;

template <class T, class U>
inline constexpr auto operator || (T&& lhs, U&& rhs) {
    return logical_or(pass<T>(lhs), pass<U>(rhs));
}

/* Comparison operations */

template <class T, class = void>
struct is_comparison : std::false_type {};

template <class T>
struct is_comparison<T, std::void_t<typename T::is_comparison>> : std::true_type {};

template <class T>
inline constexpr bool is_comparison_v = is_comparison<T>::value;

struct Equal : scalar_operation<Equal, 2, scalar_tag> {
    struct is_commutative;
    struct is_comparison;

    template <class T>
    static constexpr bool eval_scalar(T lhs, T rhs) {
        return lhs == rhs;
    }
};
inline constexpr Equal equal;

template <class T, class U>
inline constexpr auto operator == (T&& lhs, U&& rhs) {
    return equal(pass<T>(lhs), pass<U>(rhs));
}

inline constexpr auto not_equal = !equal;

template <class T, class U>
inline constexpr auto operator != (T&& lhs, U&& rhs) {
    return not_equal(pass<T>(lhs), pass<U>(rhs));
}

struct Less : scalar_operation<Less, 1, scalar_tag> {
    struct is_comparison;

    template <class T>
    static constexpr bool eval_scalar(T lhs, T rhs) {
        return lhs < rhs;
    }
};
inline constexpr Less less;

template <class T, class U>
inline constexpr auto operator < (T&& lhs, U&& rhs) {
    return less(pass<T>(lhs), pass<U>(rhs));
}

struct Cmp3Way : scalar_operation<Cmp3Way, 2, scalar_tag> {
    struct is_comparison;

    template <class T>
    static constexpr int eval_scalar(T lhs, T rhs) {
        return cast<int>(less(rhs, lhs)) - cast<int>(less(lhs, rhs));
    }
};
inline constexpr Cmp3Way cmp3way;

template <class T, class U>
inline constexpr auto operator > (T&& lhs, U&& rhs) {
    return pass<U>(rhs) < pass<T>(lhs);
}

template <class T, class U>
inline constexpr auto operator <= (T&& lhs, U&& rhs) {
    return !(pass<T>(lhs) > pass<U>(rhs));
}

template <class T, class U>
inline constexpr auto operator >= (T&& lhs, U&& rhs) {
    return !(pass<T>(lhs) < pass<U>(rhs));
}

/* Arithmetic operations */

struct Neg : scalar_operation<Neg, 2, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return -x;
    }
};
inline constexpr Neg neg;

template <class T>
inline constexpr auto operator - (T&& x) {
    return neg(pass<T>(x));
}

struct Add : scalar_operation<Add, 2, arithmetic_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct Div : scalar_operation<Div, 2, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct Mul : scalar_operation<Mul, 2, arithmetic_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct Sub : scalar_operation<Sub, 2, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct BitNot : scalar_operation<BitNot, 2, integral_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return ~x;
    }
};
inline constexpr BitNot bit_not;

template <class T>
inline constexpr auto operator ~ (T&& x) {
    return bit_not(pass<T>(x));
}

struct BitAnd : scalar_operation<BitAnd, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct BitOr : scalar_operation<BitOr, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct BitXor : scalar_operation<BitXor, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
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

struct Max : scalar_operation<Max, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
        return ternary(less(lhs, rhs), rhs, lhs);
    }
};
inline constexpr Max max;

struct Min : scalar_operation<Min, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T lhs, T rhs) {
        return ternary(less(rhs, lhs), rhs, lhs);
    }
};
inline constexpr Min min;

struct Sgn : scalar_operation<Sgn, 2, scalar_tag> {
    template <class T>
    static constexpr int eval_scalar(T x) {
        return cmp3way(x, T{0});
    }
};
inline constexpr Sgn sgn;

struct Sqr : scalar_operation<Sqr, 2, scalar_tag> {
    template <class T>
    static constexpr auto eval_scalar(T x) {
        return mul(x, x);
    }
};
inline constexpr Sqr sqr;

struct Common : scalar_operation<Common, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T eval_scalar(T x) {
        return x;
    }

    template <class T>
    static T eval_scalar(T x, T y) {
        ACTL_ASSERT(x == y);
        return x;
    }

    static constexpr auto eval_scalar(none, none) { return none{}; }

    template <class T>
    static constexpr T eval_scalar(T x, none) {
        return x;
    }

    template <class U>
    static constexpr U eval_scalar(none, U y) {
        return y;
    }

    template <class T, T X>
    static constexpr auto eval_scalar(std::integral_constant<T, X> x,
                                      std::integral_constant<T, X>) {
        return x;
    }

    template <class T, T X, class U, enable_int_if<std::is_integral_v<U>> = 0>
    static auto eval_scalar(std::integral_constant<T, X> x, U y) {
        ACTL_ASSERT(X == y);
        return x;
    }

    template <class T, class U, U Y, enable_int_if<std::is_integral_v<T>> = 0>
    static auto eval_scalar(T x, std::integral_constant<U, Y> y) {
        return eval_scalar(y, x);
    }

    template <class T0, class T1, class T2, class... Ts>
    static constexpr auto eval_scalar(T0 x0, T1 x1, T2 x2, Ts... xs) {
        return eval_scalar(eval_scalar(x0, x1), x2, xs...);
    }
};
inline constexpr Common common;

}  // namespace ac::math
