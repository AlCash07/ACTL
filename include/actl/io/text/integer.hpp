/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text_format.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac::io {

namespace detail {

template <class Char, class UInt>
inline Char* uitoa10(Char* last, UInt x, uint8_t base) {
    do {
        int digit = static_cast<int>(x % base);
        x /= base;
        *--last = static_cast<Char>('0' + digit);
    } while (x != 0);
    return last;
}

template <class Char, class UInt>
inline Char* uitoa(Char* last, UInt x, uint8_t base, Char ten) {
    do {
        int digit = static_cast<int>(x % base);
        x /= base;
        *--last = static_cast<Char>(digit < 10 ? '0' + digit : ten + (digit - 10));
    } while (x != 0);
    return last;
}

template <class T>
inline constexpr T digit_count(T x, T base) {
    return x == 0 ? 0 : 1 + digit_count(x / base, base);
}

template <uint8_t MinBase, class D, class F, class Int>
inline index write_int(D& od, F& fmt, Int x, uint8_t base) {
    using UInt = std::make_unsigned_t<Int>;
    char_t<D> s[1 + digit_count(std::numeric_limits<UInt>::max(), UInt{MinBase})];
    auto last = std::end(s);
    auto write_uint = [&](UInt x) {
        if (base <= 10) {
            last = uitoa10(last, x, base);
            if (base == 8 && x != 0 && fmt.getf(flags::showbase)) *--last = '0';
        } else {
            last = uitoa(last, x, base, fmt.getf(flags::uppercase) ? 'A' : 'a');
            if (base == 16 && x != 0 && fmt.getf(flags::showbase)) {
                *--last = fmt.getf(flags::uppercase) ? 'X' : 'x';
                *--last = '0';
            }
        }
    };
    if constexpr (std::is_signed_v<Int>) {
        UInt ux = static_cast<UInt>(x);
        if (x < 0) {
            write_uint(~ux + 1);
            *--last = '-';
        } else {
            write_uint(ux);
            if (fmt.getf(flags::showpos)) *--last = '+';
        }
    } else {
        write_uint(x);
    }
    return serialize(od, fmt, span<const char_t<D>>{last, std::end(s)}, text{});
}

}  // namespace detail

template <class Device, class Format, class Int>
inline std::enable_if_t<std::is_integral_v<Int>, index> serialize(Device& od, Format& fmt, Int x,
                                                                  text) {
    auto base = fmt.base();
    ACTL_ASSERT(2 <= base && base <= 36);
    switch (base) {
        case 10:
            return detail::write_int<10>(od, fmt, x, 10);
        case 2:
            return detail::write_int<2>(od, fmt, x, 2);
        default:
            return detail::write_int<3>(od, fmt, x, base);
    }
}

template <class Device, class Format, class Int>
inline std::enable_if_t<std::is_integral_v<Int>, bool> deserialize(Device& id, Format& fmt, Int& x,
                                                                   text) {
    return true;
}

}  // namespace ac::io
