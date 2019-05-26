/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstdint>

namespace ac::io::detail {

template <class Char, class UInt>
inline Char* uitoa10(Char* last, UInt x, uint8_t base) {
    do {
        auto digit = x % base;
        x /= base;
        *--last = static_cast<Char>('0' + digit);
    } while (x != 0);
    return last;
}

template <class Char, class UInt>
inline Char* uitoa(Char* last, UInt x, uint8_t base, Char ten) {
    do {
        auto digit = x % base;
        x /= base;
        *--last = static_cast<Char>(digit < 10 ? '0' + digit : ten + (digit - 10));
    } while (x != 0);
    return last;
}

}  // namespace ac::io::detail
