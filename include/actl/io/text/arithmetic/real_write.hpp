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
#include <actl/numeric/binary_pow.hpp>
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace ac::io {

template <class Device, class Format, class Float>
inline std::enable_if_t<std::is_floating_point_v<Float>, index> serialize(Device& od, Format& fmt,
                                                                          Float x, text_tag) {
    std::unique_ptr<char[]> s;
    char* first;
    char* last;
    auto reserve = [&](index size) {
        s = std::make_unique<char[]>(static_cast<size_t>(1 + size));
        first = last = s.get() + 1 + size;
    };
    char sign{};
    if (std::signbit(x)) {
        sign = '-';
        x = -x;
    } else {
        if (fmt.getf(flags::showpos)) sign = '+';
    }
    if (std::isnan(x)) {
        reserve(3);
        first = last - 3;
        std::memcpy(first, fmt.getf(flags::uppercase) ? "NAN" : "nan", 3);
    } else if (std::isinf(x)) {
        reserve(3);
        first = last - 3;
        std::memcpy(first, fmt.getf(flags::uppercase) ? "INF" : "inf", 3);
    } else {
        using UInt = unsigned long long;
        auto base = fmt.base();
        auto prec = fmt.precision();
        auto base_power = binary_pow(UInt{base}, prec);
        auto integer_part = static_cast<UInt>(x);
        auto fractional_part = static_cast<UInt>((x - integer_part) * base_power + Float{1} / 2);
        if (fractional_part >= base_power) {
            ++integer_part;
            fractional_part = 0;
        }
        reserve(
            detail::digit_count(std::numeric_limits<UInt>::max(), base < 10 ? UInt{2} : UInt{10}) +
            1 + fmt.precision());
        first = last - prec;
        if (prec > 0) {
            std::fill(first, detail::uitoa(last, fmt, fractional_part, base), '0');
        }
        if (0 < prec || fmt.getf(flags::showpoint)) *--first = '.';
        first = detail::uitoa(first, fmt, integer_part, base);
    }
    if (sign) *--first = sign;
    return write(od, fmt, ospan<char>{first, last});
}

}  // namespace ac::io
