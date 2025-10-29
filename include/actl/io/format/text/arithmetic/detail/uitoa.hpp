// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/format/text/text.hpp>

namespace ac::io::detail {

template<bool ShowBase = true, typename Char, typename UInt>
Char* uitoa(Char* last, TextFormat auto& fmt, UInt x, UInt base) {
    if (base <= 10) {
        bool show_base =
            ShowBase && fmt.getf(flag::showbase) && x != 0 && base == 8;
        do {
            auto digit = x % base;
            x /= base;
            *--last = static_cast<Char>('0' + digit);
        } while (x != 0);
        if (show_base)
            *--last = '0';
    } else {
        bool show_base =
            ShowBase && fmt.getf(flag::showbase) && x != 0 && base == 16;
        Char a = fmt.getf(flag::uppercase) ? 'A' : 'a';
        do {
            Char digit = static_cast<Char>(x % base);
            x /= base;
            *--last =
                static_cast<Char>(digit < 10 ? '0' + digit : a + (digit - 10));
        } while (x != 0);
        if (show_base) {
            *--last = a - 'a' + 'x';
            *--last = '0';
        }
    }
    return last;
}

} // namespace ac::io::detail
