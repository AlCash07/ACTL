/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text_format.hpp>

namespace ac::io {

template <class Device, class Format>
inline index serialize(Device& od, Format& fmt, bool x, text) {
    if (fmt.getf(flags::boolalpha)) {
        return x ? write(od, fmt, ospan{"true"}) : write(od, fmt, ospan{"false"});
    } else {
        return write(od, fmt, x ? ospan{"1"} : ospan{"0"});
    }
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, bool& x, text) {
    char_t<Device> c;
    if (!read(id, fmt, c)) return false;
    if (fmt.getf(flags::boolalpha)) {
        x = c == 't';
        switch (c) {
            case 't':
                return read(id, fmt, "rue");
            case 'f':
                return read(id, fmt, "alse");
            default:
                return false;
        }
    } else {
        x = c == '1';
        return c == '0' || c == '1';
    }
}

}  // namespace ac::io
