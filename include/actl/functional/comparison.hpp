/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/arithmetic.hpp>
#include <actl/numeric/math.hpp>
#include <actl/range/traits.hpp>
#include <algorithm>

namespace ac {

namespace op {

struct comparison_operation_tag : scalar_operation_tag {};

// Base class for scalar operations.
template <class Derived, int Arity>
struct comparison_operation : scalar_operation<Derived, Arity> {
    using operation_tag = comparison_operation_tag;
};

struct Sgn : scalar_operation<Sgn, 1> {};

struct Cmp3Way : comparison_operation<Cmp3Way, 2> {};

struct Equal : comparison_operation<Equal, 2> {
    struct is_commutative;

    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x == y) {
        return x == y;
    }
};

struct Less : comparison_operation<Less, 2> {
    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x < y) {
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

template <class Policy, class T>
inline constexpr auto perform(Sgn, const Policy& policy, const T& x) {
    return cmp3way(policy, x, 0);
}

template <class Policy, class T, class U>
inline constexpr int perform(Cmp3Way, const Policy& policy, const T& lhs, const U& rhs) {
    return (int)less(policy, rhs, lhs) - (int)less(policy, lhs, rhs);
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool perform(Equal, const Policy& policy, const R0& lhs, const R1& rhs) {
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
                      equal(policy));
}

template <class T, class U>
inline constexpr auto operator == (const T& lhs, const U& rhs) -> decltype(equal(lhs, rhs)) {
    return equal(lhs, rhs);
}

template <class T, class U>
inline constexpr bool operator != (const T& lhs, const U& rhs) {
    return !(lhs == rhs);
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool perform(Less, const Policy& policy, const R0& lhs, const R1& rhs) {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                        std::end(rhs), less(policy));
}

template <class Policy, class T, class U>
inline constexpr decltype(auto) perform(Max, const Policy& policy, const T& lhs, const U& rhs) {
    return less(policy, lhs, rhs) ? rhs : lhs;
}

template <class Policy, class T, class U>
inline constexpr decltype(auto) perform(Min, const Policy& policy, const T& lhs, const U& rhs) {
    return less(policy, rhs, lhs) ? rhs : lhs;
}

template <class T, class U>
inline constexpr auto operator < (const T& lhs, const U& rhs) -> decltype(less(lhs, rhs)) {
    return less(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator > (const T& lhs, const U& rhs) -> decltype(rhs < lhs) {
    return rhs < lhs;
}

template <class T, class U>
inline constexpr auto operator <= (const T& lhs, const U& rhs) -> decltype(!(lhs > rhs)) {
    return !(lhs > rhs);
}

template <class T, class U>
inline constexpr auto operator >= (const T& lhs, const U& rhs) -> decltype(!(lhs < rhs)) {
    return !(lhs < rhs);
}

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

template <class E, class T>
inline constexpr int perform(Sgn, const absolute_error<E>& policy, const T& x) {
    if (less(adl::abs(x), policy.epsilon())) return 0;
    return x < 0 ? -1 : 1;
}

template <class Op, class E, class T, class U,
          enable_int_if<std::is_base_of_v<comparison_operation_tag, operation_tag_t<Op>>> = 0>
inline constexpr auto perform(Op op, const absolute_error<E>& policy, const T& lhs, const U& rhs) {
    return op(sgn(policy, sub(policy, lhs, rhs)), 0);
}

}  // namespace op

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
