/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>

namespace ac::io {

struct flush_t {
    struct is_manipulator;
};
constexpr flush_t flush{};

template <class Device, class Format>
inline index write_final(Device& od, Format&, flush_t) {
    od.flush();
    return 0;
}

// Format that flushes after each unit.
struct auto_flush {
    struct format_tag;
};

template <class T>
inline auto serialize(auto_flush&, const T& x) {
    return tuple{x, flush_t{}};
}

}  // namespace ac::io
