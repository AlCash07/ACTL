/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/traits/iterator_traits.hpp>

namespace ac::io {

template <class Device, class Format, class T, class C = char_t<Device>>
inline std::enable_if_t<
    std::is_trivially_copyable_v<T> && !is_range_v<T> && sizeof(T) % sizeof(C) == 0, index>
serialize(Device& od, Format&, const T& x, binary) {
    return od.write({reinterpret_cast<const C*>(&x), sizeof(T) / sizeof(C)});
}

template <class Device, class Format, class T, class C = char_t<Device>>
inline std::enable_if_t<
    std::is_trivially_copyable_v<T> && !is_range_v<T> && sizeof(T) % sizeof(C) == 0, bool>
deserialize(Device& id, Format&, T& x, binary) {
    constexpr index XS = static_cast<index>(sizeof(T) / sizeof(C));
    return id.read({reinterpret_cast<C*>(&x), XS}) == XS;
}

}  // namespace ac::io
