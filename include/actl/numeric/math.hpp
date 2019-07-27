/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cmath>
#include <cstdlib>

namespace ac::adl {

template <class T>
inline constexpr T abs(const T& x) {
    if constexpr (std::is_unsigned_v<T>) {
        return x;
    } else {
        using std::abs;
        return abs(x);
    }
}

#define ADL1(name)                 \
    template <class T>             \
    inline auto name(const T& x) { \
        using std::name;           \
        return name(x);            \
    }

#define ADL2(name)                               \
    template <class T0, class T1>                \
    inline auto name(const T0& x, const T1& y) { \
        using std::name;                         \
        return name(x, y);                       \
    }

ADL1(sqrt)
ADL1(sin)
ADL1(cos)
ADL2(atan2)

#undef ADL1
#undef ADL2

}  // namespace ac::adl
