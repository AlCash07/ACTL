/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>
#include <type_traits>

namespace ac::io {

struct serialization_access {
    // This is the simplest portable way I found to check for optional private members in T.
    template <class T, class = typename T::composite_io_tag>
    static std::true_type has_composite_io_tag(int);

    template <class T>
    static std::false_type has_composite_io_tag(...);

    template <class T, class... Ts,
              class = decltype(std::declval<const T>().serialize(std::declval<Ts>()...))>
    static std::true_type has_serialize(int);

    template <class T, class... Ts>
    static std::false_type has_serialize(...);

    // Tag represents 0 or 1 types.
    template <class Device, class Format, class T, class... Tag>
    friend std::enable_if_t<decltype(has_serialize<T, Device, Format, Tag...>(0))::value, index>
    serialize(Device& od, Format& fmt, const T& x, Tag... tag) {
        return x.serialize(od, fmt, tag...);
    }

    template <class T, class... Ts,
              class = decltype(std::declval<T>().deserialize(std::declval<Ts>()...))>
    static std::true_type has_deserialize(int);

    template <class T, class... Ts>
    static std::false_type has_deserialize(...);

    template <class Device, class Format, class T, class... Tag>
    friend std::enable_if_t<decltype(has_deserialize<T, Device, Format, Tag...>(0))::value, bool>
    deserialize(Device& id, Format& fmt, T& x, Tag... tag) {
        return x.deserialize(id, fmt, tag...);
    }
};

}  // namespace ac::io
