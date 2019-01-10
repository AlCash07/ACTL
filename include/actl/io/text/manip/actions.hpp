/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>

namespace ac::io {

struct endl_t {} constexpr endl{};  // put '\n' and flush
struct ends_t {} constexpr ends{};  // put '\0'
struct flush_t {} constexpr flush{};

template <class Device>
inline int write(io::text, Device& od, endl_t) {
    return write(od, '\n', flush);
}

template <class Device>
inline int write(io::text, Device& od, ends_t) {
    return write(od, '\0');
}

template <class Device>
inline int write(io::text, Device& od, flush_t) {
    od.flush();
    return 0;
}

}  // namespace ac::io
