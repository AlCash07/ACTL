/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/flags.hpp>

namespace ac::io::detail {

template <bool ShowBase = true, class Char, class Format, class UInt>
inline Char* uitoa(Char* last, Format& fmt, UInt x, uint8_t base) {
    if (base <= 10) {
        bool show_base = ShowBase && fmt.getf(flags::showbase) && base == 8 && x != 0;
        do {
            auto digit = x % base;
            x /= base;
            *--last = static_cast<Char>('0' + digit);
        } while (x != 0);
        if (show_base) *--last = '0';
    } else {
        bool show_base = ShowBase && fmt.getf(flags::showbase) && base == 16 && x != 0;
        Char a = fmt.getf(flags::uppercase) ? 'A' : 'a';
        do {
            auto digit = x % base;
            x /= base;
            *--last = static_cast<Char>(digit < 10 ? '0' + digit : a + (digit - 10));
        } while (x != 0);
        if (show_base) {
            *--last = a - 'a' + 'x';
            *--last = '0';
        }
    }
    return last;
}

}  // namespace ac::io::detail
