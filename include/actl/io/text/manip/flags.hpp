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

template <flag_t flag, bool value>
struct setf {};

template <flag_t group, flag_t flag>
struct setg {};

// boolean as string or int
constexpr setf<flags::boolalpha, true> boolalpha{};
constexpr setf<flags::boolalpha, false> noboolalpha{};

// prepend integer number base prefix
constexpr setf<flags::showbase, true> showbase{};
constexpr setf<flags::showbase, false> noshowbase{};

// prepend + before positive integer and real numbers
constexpr setf<flags::showpos, true> showpos{};
constexpr setf<flags::showpos, false> noshowpos{};

// case for special characters in integer and real numbers representation
constexpr setf<flags::uppercase, true> uppercase{};
constexpr setf<flags::uppercase, false> nouppercase{};

// real numbers formatted
constexpr setg<groups::floatfield, 0> defaultfloat{};
constexpr setg<groups::floatfield, flags::fixed> fixed{};
constexpr setg<groups::floatfield, flags::scientific> scientific{};
constexpr setg<groups::floatfield, flags::hexfloat> hexfloat{};

// always show decimal point
constexpr setf<flags::showpoint, true> showpoint{};
constexpr setf<flags::showpoint, false> noshowpoint{};

// adjustment
constexpr setg<groups::adjustfield, flags::left> left{};
constexpr setg<groups::adjustfield, flags::right> right{};
constexpr setg<groups::adjustfield, flags::center> center{};

// skip whitespace before each input unit
constexpr setf<flags::skipws, true> skipws{};
constexpr setf<flags::skipws, false> noskipws{};

// flush after each unit
constexpr setf<flags::unitbuf, true> unitbuf{};
constexpr setf<flags::unitbuf, false> nounitbuf{};

template <class Device, class Format, flag_t flag, bool value>
inline int serialize(Device&, Format& fmt, setf<flag, value>, text) {
    if constexpr (value) {
        fmt.setf(flag);
    } else {
        fmt.unsetf(flag);
    }
    return 0;
}

template <class Device, class Format, flag_t flag, bool value>
inline bool deserialize(Device& id, Format& fmt, setf<flag, value>, text) {
    serialize(id, fmt, setf<flag, value>{}, text{});
    return true;
}

template <class Device, class Format, flag_t group, flag_t flag>
inline int serialize(Device&, Format& fmt, setg<group, flag>, text) {
    fmt.setf(flag, group);
    return 0;
}

template <class Device, class Format, flag_t group, flag_t flag>
inline bool deserialize(Device&, Format& fmt, setg<group, flag>, text) {
    fmt.setf(flag, group);
    return true;
}

}  // namespace ac::io
