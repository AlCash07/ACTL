/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cmath>

namespace ac { namespace math {

#define MATH_ADL1(name)            \
    template <class T>             \
    inline auto name(const T& x) { \
        using std::name;           \
        return name(x);            \
    }

#define MATH_ADL2(name)                          \
    template <class T0, class T1>                \
    inline auto name(const T0& x, const T1& y) { \
        using std::name;                         \
        return name(x, y);                       \
    }

MATH_ADL1(sqrt)
MATH_ADL1(sin)
MATH_ADL1(cos)
MATH_ADL2(atan2)

#undef MATH_ADL1
#undef MATH_ADL2

}}  // namespace ac::math
