/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/arithmetic/detail/read_digit.hpp>
#include <actl/io/text/text.hpp>
#include <actl/util/type_traits.hpp>

namespace ac::io {

namespace detail {

template <uint8_t MaxBase, auto Max, class D, class UInt>
inline bool read_uint2(D& id, UInt& x, uint8_t base) {
    UInt v, d;
    if (!read_digit<MaxBase>(id, v, base)) return false;
    const UInt safe = Max / base;
    bool overflow = false;
    while (read_digit<MaxBase>(id, d, base)) {
        if (safe < v || (safe == v && Max % base < d)) {
            v = Max;
            overflow = true;
        } else {
            v = v * base + d;
        }
    }
    x = v;
    return !overflow;
}

template <auto Max, class D, class F, class UInt>
inline bool read_uint(D& id, F& fmt, UInt& x) {
    uint8_t base = fmt.base();
    if (id.peek() == '0') {
        id.move(1);
        UInt d;
        read_digit<36>(id, d, 0);
        if (d == 'x' - 'a' + 10) {
            if (base == 0) base = 16;
            if (base == 16) {
                d = 0;
                id.move(1);
            }
        } else if (base == 0) {
            base = 8;
        }
        if (d >= base) {
            x = 0;
            return true;
        }
    } else {
        if (base == 0) base = 10;
    }
    if (base <= 10) {
        return read_uint2<10, Max>(id, x, base);
    } else {
        return read_uint2<36, Max>(id, x, base);
    }
}

}  // namespace detail

template <class Device, class Format, class Int,
          enable_int_if<std::is_integral_v<Int> && !std::is_same_v<Int, char_t<Device>>> = 0>
inline bool deserialize(Device& id, Format& fmt, Int& x, text_tag) {
    if (fmt.getf(flags::skipws)) read(id, fmt, ws);
    if constexpr (std::is_signed_v<Int>) {
        using UInt = std::make_unsigned_t<Int>;
        UInt ux;
        bool ok;
        if (id.peek() == '-') {
            id.move(1);
            ok = detail::read_uint<std::numeric_limits<UInt>::max() / 2 + 1>(id, fmt, ux);
            x = ~static_cast<Int>(ux - 1);
        } else {
            if (id.peek() == '+') id.move(1);
            ok = detail::read_uint<std::numeric_limits<UInt>::max() / 2>(id, fmt, ux);
            x = static_cast<Int>(ux);
        }
        return ok;
    } else {
        if (id.peek() == '+') id.move(1);
        return detail::read_uint<std::numeric_limits<Int>::max()>(id, fmt, x);
    }
}

}  // namespace ac::io
