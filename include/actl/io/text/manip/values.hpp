/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>

namespace ac::io {

// integer and real numbers base
struct setbase {
    uint8_t value;
    constexpr setbase(uint8_t v = 10) : value{v} {}
};
constexpr setbase dec(10);
constexpr setbase hex(16);
constexpr setbase oct(8);

// number of digits after the decimal point
struct setprecision {
    uint8_t value;
    setprecision(uint8_t v = 6) : value{v} {}
};

// minimum width of an output unit
struct setwidth {
    uint8_t value;
    setwidth(uint8_t v = 0) : value{v} {}
};

// character to pad units with less width
struct setfill {
    char value;
    setfill(char v = ' ') : value{v} {}
};

// units delimiter, inserted between consecutive non-string and non-char output units
struct setdelimiter {
    const char* value;
    setdelimiter(const char* v) : value{v} {}
};

template <class Device>
inline bool read(io::text, Device& id, setbase arg) {
    id.base(arg.value);
    return true;
}

#define WRITE_MANIP(name)                                   \
    template <class Device>                                 \
    inline int write(io::text, Device& od, set##name arg) { \
        od.name(arg.value);                                 \
        return 0;                                           \
    }

WRITE_MANIP(base)
WRITE_MANIP(precision)
WRITE_MANIP(width)
WRITE_MANIP(fill)
WRITE_MANIP(delimiter)

#undef WRITE_MANIP

}  // namespace ac::io
