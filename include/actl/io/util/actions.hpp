/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/util/raw.hpp>

namespace ac::io {

struct endl_t {
    struct is_manipulator;
};
constexpr endl_t endl{};   // put '\n' and flush
constexpr raw ends{'\0'};  // put '\0'
struct flush_t {
    struct is_manipulator;
};
constexpr flush_t flush{};

template <class Format>
inline auto serialize(Format& fmt, endl_t) {
    return tuple{raw{'\n'}, flush};
}

template <class Device, class Format>
inline index write_final(Device& od, Format&, flush_t) {
    od.flush();
    return 0;
}

}  // namespace ac::io
