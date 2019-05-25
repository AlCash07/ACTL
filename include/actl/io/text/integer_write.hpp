/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text_format.hpp>
#include <type_traits>

namespace ac::io {

namespace detail {

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

template <class T>
inline constexpr T digit_count(T x, T base) {
    return x == 0 ? 0 : 1 + digit_count(x / base, base);
}

template <class Char, class F, class UInt>
inline Char* write_uint(Char* last, F& fmt, UInt x, uint8_t base) {
    if (base <= 10) {
        last = uitoa10(last, x, base);
        if (base == 8 && x != 0 && fmt.getf(flags::showbase)) *--last = '0';
    } else {
        Char ten = fmt.getf(flags::uppercase) ? 'A' : 'a';
        last = uitoa(last, x, base, ten);
        if (base == 16 && x != 0 && fmt.getf(flags::showbase)) {
            *--last = ten + 'x' - 'a';
            *--last = '0';
        }
    }
    return last;
}

template <uint8_t MinBase, class D, class F, class Int>
inline index write_int(D& od, F& fmt, Int x, uint8_t base) {
    using UInt = std::make_unsigned_t<Int>;
    char_t<D> s[1 + digit_count(std::numeric_limits<UInt>::max(), UInt{MinBase})];
    auto last = std::end(s);
    if constexpr (std::is_signed_v<Int>) {
        if (x < 0) {
            last = write_uint(last, fmt, ~static_cast<UInt>(x) + UInt{1}, base);
            *--last = '-';
        } else {
            last = write_uint(last, fmt, x, base);
            if (fmt.getf(flags::showpos)) *--last = '+';
        }
    } else {
        last = write_uint(last, fmt, x, base);
    }
    return write(od, fmt, span<const char_t<D>>{last, std::end(s)});
}

}  // namespace detail

template <class Device, class Format, class Int>
inline std::enable_if_t<std::is_integral_v<Int>, index> serialize(Device& od, Format& fmt, Int x,
                                                                  text) {
    auto base = fmt.base();
    ACTL_ASSERT(base == 0 || (2 <= base && base <= 36));
    if (base == 0 || base == 10) {
        return detail::write_int<10>(od, fmt, x, 10);
    } else if (base < 7) {
        return detail::write_int<2>(od, fmt, x, base);
    } else {
        return detail::write_int<7>(od, fmt, x, base);
    }
}

}  // namespace ac::io
