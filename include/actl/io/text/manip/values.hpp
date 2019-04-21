/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/macros.hpp>

namespace ac::io {

// integer and real numbers base
struct setbase {
    uint8_t value;
    explicit constexpr setbase(uint8_t v = 10) : value{v} {}
};
constexpr setbase dec(10);
constexpr setbase hex(16);
constexpr setbase oct(8);

// number of digits after the decimal point
struct setprecision {
    uint8_t value;
    explicit constexpr setprecision(uint8_t v = 6) : value{v} {}
};

// minimum width of an output unit
struct setwidth {
    uint8_t value;
    explicit constexpr setwidth(uint8_t v = 0) : value{v} {}
};

// character to pad units with less width
struct setfill {
    char value;
    explicit constexpr setfill(char v = ' ') : value{v} {}
};

template <class Device, class Format>
inline bool deserialize(Device&, Format& fmt, setbase arg, text) {
    fmt.base(arg.value);
    return true;
}

#define SERIALIZE_MANIP(name)                                              \
    template <class Device, class Format>                                  \
    inline int serialize(Device&, Format& fmt, CAT(set, name) arg, text) { \
        fmt.name(arg.value);                                               \
        return 0;                                                          \
    }

SERIALIZE_MANIP(base)
SERIALIZE_MANIP(precision)
SERIALIZE_MANIP(width)
SERIALIZE_MANIP(fill)

#undef SERIALIZE_MANIP

}  // namespace ac::io
