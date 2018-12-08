/***************************************************************************************************
 * I/O manipulators analogous to https://en.cppreference.com/w/cpp/io/manip
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/formatted.hpp>

namespace ac::io {

template <flag_t flag, bool value>
struct setf {};

template <flag_t group, flag_t flag>
struct setg {};

// boolean as string or int
constexpr setf<flags::boolalpha, true>  boolalpha{};
constexpr setf<flags::boolalpha, false> noboolalpha{};

// prepend integer number base prefix
constexpr setf<flags::showbase, true>  showbase{};
constexpr setf<flags::showbase, false> noshowbase{};

// integer and real numbers base
struct setbase {
    uint8_t value;
    constexpr setbase(uint8_t v = 10) : value{v} {}
};
constexpr setbase dec(10);
constexpr setbase hex(16);
constexpr setbase oct(8);

// prepend + before positive integer and real numbers
constexpr setf<flags::showpos, true>  showpos{};
constexpr setf<flags::showpos, false> noshowpos{};

// case for special characters in integer and real numbers representation
constexpr setf<flags::uppercase, true>  uppercase{};
constexpr setf<flags::uppercase, false> nouppercase{};

// real numbers formatted
constexpr setg<groups::floatfield, 0>                 defaultfloat{};
constexpr setg<groups::floatfield, flags::fixed>      fixed{};
constexpr setg<groups::floatfield, flags::scientific> scientific{};
constexpr setg<groups::floatfield, flags::hexfloat>   hexfloat{};

// always show decimal point
constexpr setf<flags::showpoint, true>  showpoint{};
constexpr setf<flags::showpoint, false> noshowpoint{};

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

// adjustment
constexpr setg<groups::adjustfield, flags::left>     left{};
constexpr setg<groups::adjustfield, flags::right>    right{};
constexpr setg<groups::adjustfield, flags::center>   center{};
constexpr setg<groups::adjustfield, flags::internal> internal{};

// skip whitespace before each input unit
constexpr setf<flags::skipws, true>  skipws{};
constexpr setf<flags::skipws, false> noskipws{};

// units delimiter, inserted between consecutive non-string and non-char output units
struct setdelimiter {
    const char* value;
    setdelimiter(const char* v) : value{v} {}
};

// flush after each unit
constexpr setf<flags::unitbuf, true>  unitbuf{};
constexpr setf<flags::unitbuf, false> nounitbuf{};

// enclose containers in brackets, don't output their size
constexpr setf<flags::brackets, true>  brackets{};
constexpr setf<flags::brackets, false> nobrackets{};

// actions
struct endl_t  {} constexpr endl{};  // put '\n' and flush
struct ends_t  {} constexpr ends{};  // put '\0'
struct flush_t {} constexpr flush{};
struct ws_t    {} constexpr ws{};  // skip whitespace

template <class Device, flag_t flag, bool value>
inline bool read(Device& dr, setf<flag, value>) {
    if constexpr (value) {
        dr.setf(flag);
    } else {
        dr.unseft(flag);
    }
    return true;
}

template <class Device, flag_t flag, bool value>
inline int write(Device& dw, setf<flag, value>) {
    read(dw, setf<flag, value>{});
    return 0;
}

template <class Device, flag_t group, flag_t flag>
inline bool read(Device& dr, setg<group, flag>) {
    dr.setf(flag, group);
    return true;
}

template <class Device, flag_t group, flag_t flag>
inline int write(Device& dw, setg<group, flag>) {
    dw.setf(flag, group);
    return 0;
}

template <class Device>
inline bool read(Device& dr, setbase arg) {
    dr.base(arg.value);
    return true;
}

#define WRITE_MANIP(name)                         \
    template <class Device>                       \
    inline int write(Device& dw, set##name arg) { \
        dw.name(arg.value);                       \
        return 0;                                 \
    }

WRITE_MANIP(base)
WRITE_MANIP(precision)
WRITE_MANIP(width)
WRITE_MANIP(fill)
WRITE_MANIP(delimiter)

#undef WRITE_MANIP

template <class Device>
inline int write(Device& dw, endl_t) {
    return write(dw, '\n', flush);
}

template <class Device>
inline int write(Device& dw, ends_t) {
    return write(dw, '\0');
}

template <class Device>
inline int write(Device& dw, flush_t) {
    dw.flush();
    return 0;
}

template <class Device>
inline bool read(Device& dr, ws_t) {
    // implement
    return true;
}

}  // namespace ac::io
