/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/policy.hpp>

namespace ac {

namespace op {

DEFINE_HAS_BINARY_OPERATOR(mul, *)
DEFINE_HAS_OVERLOAD(product)

template <class... Ts>
using product_t = decltype(product(std::declval<Ts>()...));

template <class T, class U, enable_int_if<!has_product_v<T, U> && has_mul_v<T, U>> = 0>
inline constexpr auto product(policy, const T& lhs, const U& rhs) {
    return lhs * rhs;
}

template <class P>
struct product_policy : virtual policy {};

template <class P, class T, class U,
          enable_int_if<std::is_convertible_v<T, P> && std::is_convertible_v<U, P>> = 0>
inline constexpr auto product(product_policy<P>, const T& lhs, const U& rhs) {
    return static_cast<P>(lhs) * static_cast<P>(rhs);
}

template <class T, class U, enable_int_if<has_product_v<T, U>> = 0>
inline constexpr auto operator * (const T& lhs, const U& rhs) {
    return product(default_policy, lhs, rhs);
}

DEFINE_HAS_BINARY_OPERATOR(div, <)
DEFINE_HAS_OVERLOAD(ratio)

template <class... Ts>
using ratio_t = decltype(ratio(std::declval<Ts>()...));

template <class T, class U, enable_int_if<!has_ratio_v<T, U> && has_div_v<T, U>> = 0>
inline constexpr auto ratio(policy, const T& lhs, const U& rhs) {
    return lhs / rhs;
}

template <class R>
struct ratio_policy : virtual policy {};

template <class R, class T, class U,
          enable_int_if<std::is_convertible_v<T, R> && std::is_convertible_v<U, R>> = 0>
inline constexpr auto ratio(ratio_policy<R>, const T& lhs, const U& rhs) {
    return static_cast<R>(lhs) / static_cast<R>(rhs);
}

template <class T, class U, enable_int_if<has_ratio_v<T, U>> = 0>
inline constexpr auto operator / (const T& lhs, const U& rhs) {
    return ratio(default_policy, lhs, rhs);
}

}  // namespace op

template <class Policy, class T>
inline constexpr auto sqr(const Policy& policy, const T& x) {
    return product(policy, x, x);
}

template <class T>
inline constexpr auto sqr(const T& x) {
    return x * x;
}

}  // namespace ac
