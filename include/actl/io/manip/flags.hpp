/***************************************************************************************************
 * I/O manipulators analogous to https://en.cppreference.com/w/cpp/io/manip
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
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

// enclose containers in brackets, don't output their size
constexpr setf<flags::brackets, true> brackets{};
constexpr setf<flags::brackets, false> nobrackets{};

template <class Device, flag_t flag, bool value>
inline bool read(io::text, Device& rd, setf<flag, value>) {
    if constexpr (value) {
        rd.setf(flag);
    } else {
        rd.unsetf(flag);
    }
    return true;
}

template <class Device, flag_t flag, bool value>
inline int write(io::text, Device& wd, setf<flag, value>) {
    read(wd, setf<flag, value>{});
    return 0;
}

template <class Device, flag_t group, flag_t flag>
inline bool read(io::text, Device& rd, setg<group, flag>) {
    rd.setf(flag, group);
    return true;
}

template <class Device, flag_t group, flag_t flag>
inline int write(io::text, Device& wd, setg<group, flag>) {
    wd.setf(flag, group);
    return 0;
}

}  // namespace ac::io
