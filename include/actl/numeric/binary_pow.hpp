/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/bit.hpp>

namespace ac {

template <class T, class Int>
inline T binary_pow(T value, Int power) {
    ACTL_ASSERT(power >= 0);
    T res{1};
    if (power == 0) return res;
    for (; power > 1; power >>= 1) {
        if (power & 1) res *= value;
        value *= value;
    }
    return res * value;
}

}  // namespace ac
