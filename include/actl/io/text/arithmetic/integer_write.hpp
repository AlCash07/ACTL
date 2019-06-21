/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/arithmetic/detail/digit_count.hpp>
#include <actl/io/text/arithmetic/detail/uitoa.hpp>
#include <actl/io/text/text.hpp>
#include <type_traits>

namespace ac::io {

namespace detail {

template <uint8_t MinBase, class D, class F, class Int>
inline index write_int(D& od, F& fmt, Int x, uint8_t base) {
    using UInt = std::make_unsigned_t<Int>;
    char_t<D> s[1 + digit_count(std::numeric_limits<UInt>::max(), UInt{MinBase})];
    auto last = std::end(s);
    if constexpr (std::is_signed_v<Int>) {
        if (x < 0) {
            last = uitoa(last, fmt, ~static_cast<UInt>(x) + UInt{1}, base);
            *--last = '-';
        } else {
            last = uitoa(last, fmt, x, base);
            if (fmt.getf(flags::showpos)) *--last = '+';
        }
    } else {
        last = uitoa(last, fmt, x, base);
    }
    return write(od, fmt, cspan<char_t<D>>{last, std::end(s)});
}

}  // namespace detail

template <
    class Device, class Format, class Int,
    std::enable_if_t<std::is_integral_v<Int> && !std::is_same_v<Int, char_t<Device>>, int> = 0>
inline index serialize(Device& od, Format& fmt, Int x, text_tag) {
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
