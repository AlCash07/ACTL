/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text.hpp>

namespace ac::io {

constexpr char true_s[4] = {'t', 'r', 'u', 'e'};
constexpr char false_s[5] = {'f', 'a', 'l', 's', 'e'};

template <class Device, class Format>
inline index serialize(Device& od, Format& fmt, bool x, text_tag) {
    if (fmt.getf(flags::boolalpha)) {
        return x ? write(od, fmt, span{true_s}) : write(od, fmt, span{false_s});
    } else {
        const char c = x ? '1' : '0';
        return write(od, fmt, span{&c, 1});
    }
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, bool& x, text_tag) {
    char_t<Device> c;
    if (!read(id, fmt, c)) return false;
    if (fmt.getf(flags::boolalpha)) {
        x = c == 't';
        switch (c) {
            case 't':
                return read(id, fmt, span{std::begin(true_s) + 1, std::end(true_s)});
            case 'f':
                return read(id, fmt, span{std::begin(false_s) + 1, std::end(false_s)});
            default:
                return false;
        }
    } else {
        x = c == '1';
        return c == '0' || c == '1';
    }
}

}  // namespace ac::io
