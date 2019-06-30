/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/arithmetic/detail/digit_count.hpp>
#include <actl/io/text/arithmetic/detail/peek_digit.hpp>
#include <actl/io/text/text.hpp>
#include <actl/numeric/algorithm/binary_pow.hpp>
#include <actl/std/utility.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac::io {

template <class Device, class Format, class Float,
          enable_int_if<std::is_floating_point_v<Float>> = 0>
inline bool deserialize(Device& id, Format& fmt, Float& x, text_tag) {
    if (fmt.getf(flags::skipws)) read(id, fmt, ws);
    char c = id.peek();
    bool negative = c == '-';
    if (negative || c == '+') id.move(1);
    auto base = fmt.base();
    if (base == 0) base = 10;
    using UInt = unsigned long long;
    const index max_length = detail::digit_count(std::numeric_limits<UInt>::max(), UInt{base}) - 1;
    auto read_part = [&]() {
        UInt t{};
        index length{};
        unsigned int d;
        while (length < max_length && detail::peek_digit<36>(id, d, base)) {
            id.move(1);
            t = t * base + d;
            ++length;
        }
        return std::pair{t, length};
    };
    if (auto [p, length] = read_part(); length > 0) {
        x = static_cast<Float>(p);
    } else {
        return false;
    }
    while (true) {
        auto [p, length] = read_part();
        if (length == 0) break;
        x = x * binary_pow(UInt{base}, length) + p;
    }
    if (id.peek() == '.') {
        id.move(1);
        Float power{1};
        while (true) {
            auto [p, length] = read_part();
            if (length == 0) break;
            power /= binary_pow(UInt{base}, length);
            x += p * power;
        }
    }
    if (negative) x = -x;
    return true;
}

}  // namespace ac::io
