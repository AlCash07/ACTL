/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>

namespace ac::io {

struct endl_t {};
constexpr endl_t endl{};  // put '\n' and flush
struct ends_t {};
constexpr ends_t ends{};  // put '\0'
struct flush_t {};
constexpr flush_t flush{};

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format& fmt, endl_t, Tag) {
    return od.put('\n') + serialize(od, fmt, flush, Tag{});
}

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format& fmt, ends_t, Tag) {
    return od.put('\0');
}

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format&, flush_t, Tag) {
    od.flush();
    return 0;
}

}  // namespace ac::io
