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
constexpr char zero_s[1] = {'0'};
constexpr char one_s[1] = {'1'};

template <class Format, class Bool, enable_int_if_text<Format> = 0,
          enable_int_if<std::is_same_v<Bool, bool>> = 0>
inline cspan<char> serialize(Format& fmt, Bool x) {
    if (fmt.getf(flags::boolalpha)) {
        if (x) {
            return true_s;
        } else {
            return false_s;
        }
    } else {
        if (x) {
            return one_s;
        } else {
            return zero_s;
        }
    }
}

template <class Device, class Format, enable_int_if_text<Format> = 0>
inline bool deserialize(Device& id, Format& fmt, bool& x) {
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
