/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/manip/raw.hpp>

namespace ac::io {

struct endl_t {};
constexpr endl_t endl{};  // put '\n' and flush
constexpr raw ends{'\0'};  // put '\0'
struct flush_t {};
constexpr flush_t flush{};

template <>
struct is_manipulator<endl_t> : std::true_type {};

template <>
struct is_manipulator<flush_t> : std::true_type {};

template <class Device, class Format>
inline index serialize(Device& od, Format& fmt, endl_t) {
    return write(od, fmt, raw{'\n'}, flush);
}

template <class Device, class Format>
inline index serialize(Device& od, Format&, flush_t) {
    od.flush();
    return 0;
}

}  // namespace ac::io
