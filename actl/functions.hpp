/***********************************************************************************************//**
 * Copyright (c) Oleksandr Bacherikov 2017.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac {

template <class T>
inline constexpr T& smin(T& x, const T& y) {
    return y < x ? x = y : x;
}

template <class T>
inline constexpr T& smax(T& x, const T& y) {
    return x < y ? x = y : x;
}

template <class T>
inline constexpr T abs(const T& x) {
    return x < T{0} ? -x : x;
}

template <class T>
inline constexpr int sgn(const T& x) {
    return (T{0} < x) - (x < T{0});
}

template <class T>
inline constexpr T sqr(const T& x) {
    return x * x;
}

}  // namespace ac
