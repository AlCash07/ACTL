// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_traits.hpp>
#include <actl/numeric/bit.hpp>
#include <actl/std/array.hpp>

namespace ac::io {

template <
    class Device,
    class Format,
    class T,
    enable_int_if<std::is_arithmetic_v<T>> = 0>
bool serialize(Device& od, Format&, const T& x, binary)
{
    auto bytes = bit_cast<std::array<std::byte, sizeof(T)>>(x);
    return od.write(bytes) == static_cast<index>(bytes.size());
}

template <
    class Device,
    class Format,
    class T,
    enable_int_if<std::is_arithmetic_v<T>> = 0>
bool deserialize(Device& id, Format&, T& x, binary)
{
    std::array<std::byte, sizeof(T)> bytes;
    bool ok = id.read(bytes) == static_cast<index>(bytes.size());
    if (ok)
        x = bit_cast<T>(bytes);
    return ok;
}

} // namespace ac::io
