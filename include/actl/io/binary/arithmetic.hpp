/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/numeric/bit.hpp>
#include <actl/std/array.hpp>

namespace ac::io {

template <class Device, class Format, class T, class C = char_t<Device>>
inline std::enable_if_t<std::is_arithmetic_v<T> && sizeof(T) % sizeof(C) == 0, index> serialize(
    Device& od, Format&, const T& x, binary) {
    auto bytes = bit_cast<std::array<C, sizeof(T) / sizeof(C)>>(x);
    return od.write(bytes);
}

template <class Device, class Format, class T, class C = char_t<Device>>
inline std::enable_if_t<std::is_arithmetic_v<T> && sizeof(T) % sizeof(C) == 0, bool> deserialize(
    Device& id, Format&, T& x, binary) {
    std::array<C, sizeof(T) / sizeof(C)> bytes;
    bool ok = id.read(bytes) == static_cast<index>(bytes.size());
    if (ok) x = bit_cast<T>(bytes);
    return ok;
}

}  // namespace ac::io
