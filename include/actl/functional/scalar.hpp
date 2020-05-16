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

template <class T>
inline constexpr bool is_scalar_operation_v =
    std::is_base_of_v<scalar_tag, typename T::arguments_tag>;

template <class Op>
struct scalar_calculator {
    template <class Policy, class... Ts>
    static auto can_eval(const Policy& policy, const Ts&... xs)
        -> decltype(eval(perform_policy(Op{}, policy, eval(xs, policy)...), policy),
                    std::true_type{});

    static auto can_eval(...) -> std::false_type;

    template <class Policy, class... Ts>
    static constexpr decltype(auto) evaluate(const Policy& policy, const Ts&... xs) {
        return eval(perform_policy(Op{}, policy, eval(xs, policy)...), policy);
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

template <class Op, enable_int_if<is_scalar_operation_v<Op>> = 0>
inline scalar_calculator<Op> get_calculator(Op, scalar_tag);

template <class To>
struct Cast : operation<Cast<To>, 1, arithmetic_tag> {
    template <class T>
    static constexpr auto perform(T x) -> decltype(static_cast<To>(x)) {
        return static_cast<To>(x);
    }
};
template <class T>
inline constexpr Cast<T> cast;

struct Copy : operation<Copy, 1, arithmetic_tag> {
    template <class T>
    static constexpr T perform(T x) {
        return x;
    }
};
inline constexpr Copy copy;

struct Ternary : operation<Ternary, 3, arithmetic_tag> {
    template <class T>
    static constexpr auto perform(bool condition, T lhs, T rhs)
        -> remove_cvref_t<decltype(condition ? lhs : rhs)> {
        return condition ? lhs : rhs;
    }
};
inline constexpr Ternary ternary;

/* Logical operations */

struct LogicalNot : operation<LogicalNot, 1, boolean_tag> {
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

struct LogicalAnd : operation<LogicalAnd, 2, boolean_tag> {
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

struct LogicalOr : operation<LogicalOr, 2, boolean_tag> {
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

template <class T, class = void>
struct is_comparison : std::false_type {};

template <class T>
struct is_comparison<T, std::void_t<typename T::is_comparison>> : std::true_type {};

template <class T>
inline constexpr bool is_comparison_v = is_comparison<T>::value;

struct Equal : operation<Equal, 2, scalar_tag> {
    struct is_commutative;
    struct is_comparison;

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

struct Less : operation<Less, 1, scalar_tag> {
    struct is_comparison;

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

struct Cmp3Way : operation<Cmp3Way, 2, scalar_tag> {
    struct is_comparison;

    // TODO: figure out a way to correctly handle rvalues here.
    template <class T, class U>
    static constexpr auto perform(const T& lhs, const U& rhs) {
        return cast<int>(less(rhs, lhs)) - cast<int>(less(lhs, rhs));
    }
};
inline constexpr Cmp3Way cmp3way;

template <class T, class U>
inline constexpr auto operator != (T&& lhs, U&& rhs) {
    return !(pass<T>(lhs) == pass<U>(rhs));
}

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

struct Neg : operation<Neg, 2, arithmetic_tag> {
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

struct Add : operation<Add, 2, arithmetic_tag> {
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

struct Div : operation<Div, 2, arithmetic_tag> {
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

struct Mul : operation<Mul, 2, arithmetic_tag> {
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

struct Sub : operation<Sub, 2, arithmetic_tag> {
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

struct BitNot : operation<BitNot, 2, integral_tag> {
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

struct BitAnd : operation<BitAnd, 2, integral_tag> {
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

struct BitOr : operation<BitOr, 2, integral_tag> {
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

struct BitXor : operation<BitXor, 2, integral_tag> {
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

struct Max : operation<Max, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto perform(const T& lhs, const U& rhs) {
        return ternary(less(lhs, rhs), rhs, lhs);
    }
};
inline constexpr Max max;

struct Min : operation<Min, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto perform(const T& lhs, const U& rhs) {
        return ternary(less(rhs, lhs), rhs, lhs);
    }
};
inline constexpr Min min;

struct Sgn : operation<Sgn, 2, scalar_tag> {
    template <class T>
    static constexpr auto perform(const T& x) {
        return cmp3way(x, T{0});
    }
};
inline constexpr Sgn sgn;

struct Sqr : operation<Sqr, 2, scalar_tag> {
    template <class T>
    static constexpr auto perform(const T& x) {
        return mul(x, x);
    }
};
inline constexpr Sqr sqr;

struct Common : operation<Common, 2, scalar_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T>
    static constexpr T perform(T x) {
        return x;
    }

    template <class T>
    static T perform(T x, T y) {
        ACTL_ASSERT(x == y);
        return x;
    }

    static constexpr auto perform(none, none) { return none{}; }

    template <class T>
    static constexpr T perform(T x, none) {
        return x;
    }

    template <class U>
    static constexpr U perform(none, U y) {
        return y;
    }

    template <class T, T X>
    static constexpr auto perform(std::integral_constant<T, X> x, std::integral_constant<T, X>) {
        return x;
    }

    template <class T, T X, class U, enable_int_if<std::is_integral_v<U>> = 0>
    static auto perform(std::integral_constant<T, X> x, U y) {
        ACTL_ASSERT(X == y);
        return x;
    }

    template <class T, class U, U Y, enable_int_if<std::is_integral_v<T>> = 0>
    static auto perform(T x, std::integral_constant<U, Y> y) {
        return perform(y, x);
    }

    template <class T0, class T1, class T2, class... Ts>
    static constexpr auto perform(T0 x0, T1 x1, T2 x2, Ts... xs)
        -> decltype(Common::perform(Common::perform(x0, x1), x2, xs...)) {
        return Common::perform(Common::perform(x0, x1), x2, xs...);
    }
};
inline constexpr Common common;

}  // namespace ac::math
