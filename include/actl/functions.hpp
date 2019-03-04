/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

template <class T>
inline constexpr T abs(const T& x) {
    if constexpr (std::is_unsigned_v<T>) {
        return x;
    } else {
        return x < T{0} ? -x : x;
    }
}

template <class T>
inline constexpr int sgn(const T& x, const T& y = T{0}) {
    return (y < x) - (x < y);
}

template <class T>
inline constexpr T& smax(T& x, const T& y) {
    return x < y ? x = y : x;
}

template <class T>
inline constexpr T& smin(T& x, const T& y) {
    return y < x ? x = y : x;
}

template <class T>
inline constexpr T sqr(const T& x) {
    return x * x;
}

}  // namespace ac
