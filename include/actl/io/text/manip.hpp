/***************************************************************************************************
 * I/O manipulators analogous to https://en.cppreference.com/w/cpp/io/manip
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/text/flags.hpp>

namespace ac::io {

template <flag_t Flag, bool Value>
struct setf {};

template <flag_t Group, flag_t Flag>
struct setg {};

// boolean as string or int
constexpr setf<flags::boolalpha, true> boolalpha{};
constexpr setf<flags::boolalpha, false> noboolalpha{};

// prepend integer number base prefix
constexpr setf<flags::showbase, true> showbase{};
constexpr setf<flags::showbase, false> noshowbase{};

// prepend '+' before positive integer and real numbers
constexpr setf<flags::showpos, true> showpos{};
constexpr setf<flags::showpos, false> noshowpos{};

// case for special characters in integer and real numbers representation
constexpr setf<flags::uppercase, true> uppercase{};
constexpr setf<flags::uppercase, false> nouppercase{};

// real numbers format
constexpr setg<groups::floatfield, 0> defaultfloat{};
constexpr setg<groups::floatfield, flags::fixed> fixed{};
constexpr setg<groups::floatfield, flags::scientific> scientific{};
constexpr setg<groups::floatfield, flags::hexfloat> hexfloat{};

// always show decimal point
constexpr setf<flags::showpoint, true> showpoint{};
constexpr setf<flags::showpoint, false> noshowpoint{};

// skip whitespace before each input unit
constexpr setf<flags::skipws, true> skipws{};
constexpr setf<flags::skipws, false> noskipws{};

// flush after each unit
constexpr setf<flags::unitbuf, true> unitbuf{};
constexpr setf<flags::unitbuf, false> nounitbuf{};

template <class Device, class Format, flag_t Flag, bool Value, class Tag>
inline index serialize(Device&, Format& fmt, setf<Flag, Value>, Tag) {
    if constexpr (Value) {
        fmt.setf(Flag);
    } else {
        fmt.unsetf(Flag);
    }
    return 0;
}

template <class Device, class Format, flag_t Flag, bool Value, class Tag>
inline bool deserialize(Device& id, Format& fmt, setf<Flag, Value>, Tag) {
    serialize(id, fmt, setf<Flag, Value>{}, Tag{});
    return true;
}

template <class Device, class Format, flag_t Group, flag_t Flag, class Tag>
inline index serialize(Device&, Format& fmt, setg<Group, Flag>, Tag) {
    fmt.setf(Flag, Group);
    return 0;
}

template <class Device, class Format, flag_t Group, flag_t Flag, class Tag>
inline bool deserialize(Device&, Format& fmt, setg<Group, Flag>, Tag) {
    fmt.setf(Flag, Group);
    return true;
}

// integer and real numbers base
struct setbase {
    uint8_t value = 10;
};
constexpr setbase dec{10};
constexpr setbase hex{16};
constexpr setbase oct{8};

template <class Device, class Format, class Tag>
inline bool deserialize(Device&, Format& fmt, setbase x, Tag) {
    fmt.base(x.value);
    return true;
}

template <class Device, class Format, class Tag>
inline index serialize(Device&, Format& fmt, setbase x, Tag) {
    fmt.base(x.value);
    return 0;
}

// number of digits after the decimal point
struct setprecision {
    index value = 6;
};

template <class Device, class Format, class Tag>
inline index serialize(Device&, Format& fmt, setprecision x, Tag) {
    fmt.precision(x.value);
    return 0;
}

}  // namespace ac::io
