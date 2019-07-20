/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/numeric/math.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E {
    struct is_policy;
};

namespace op {

template <class T>
inline constexpr bool equal(use_default, const T& lhs, const T& rhs) {
    return lhs == rhs;
}

template <class E, class T>
inline constexpr bool equal(const absolute_error<E>& policy, const T& lhs, const T& rhs) {
    return adl::abs(rhs - lhs) < policy.epsilon();
}

template <class T>
inline constexpr bool less(use_default, const T& lhs, const T& rhs) {
    return lhs < rhs;
}

template <class E, class T>
inline constexpr bool less(const absolute_error<E>& policy, const T& lhs, const T& rhs) {
    return policy.epsilon() < rhs - lhs;
}

}  // namespace op

template <class Policy, class T>
inline constexpr int sgn(const Policy& policy, const T& x, const T& y = T{0}) {
    return op::less(policy, y, x) - op::less(policy, x, y);
}

template <class T>
inline constexpr int sgn(const T& x, const T& y = T{0}) {
    return sgn(use_default{}, x, y);
}

template <class Policy, class T>
inline constexpr T& smax(const Policy& policy, T& x, const T& y) {
    return op::less(policy, x, y) ? x = y : x;
}

template <class T>
inline constexpr T& smax(T& x, const T& y) {
    return smax(use_default{}, x, y);
}

template <class Policy, class T>
inline constexpr T& smin(const Policy& policy, T& x, const T& y) {
    return op::less(policy, y, x) ? x = y : x;
}

template <class T>
inline constexpr T& smin(T& x, const T& y) {
    return smin(use_default{}, x, y);
}

}  // namespace ac
