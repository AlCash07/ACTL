/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>

namespace ac::io {

struct endl_t {} constexpr endl{};  // put '\n' and flush
struct ends_t {} constexpr ends{};  // put '\0'
struct flush_t {} constexpr flush{};

template <class Device, class Format>
inline index serialize(Device& od, Format& fmt, endl_t) {
    return write(od, fmt, '\n', flush);
}

template <class Device, class Format>
inline index serialize(Device& od, Format& fmt, ends_t) {
    return write(od, fmt, '\0');
}

template <class Device, class Format>
inline index serialize(Device& od, Format&, flush_t) {
    od.flush();
    return 0;
}

}  // namespace ac::io
