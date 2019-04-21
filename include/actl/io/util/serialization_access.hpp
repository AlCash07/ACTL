/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac::io {

template <class AlwaysVoid, class T, class... Ts>
struct has_serialize : std::false_type {};

template <class T, class... Ts>
struct has_serialize<
    std::void_t<decltype(std::declval<const T>().serialize(std::declval<Ts>()...))>, T, Ts...>
    : std::true_type {};

template <class AlwaysVoid, class T, class... Ts>
struct has_deserialize : std::false_type {};

template <class T, class... Ts>
struct has_deserialize<std::void_t<decltype(std::declval<T>().deserialize(std::declval<Ts>()...))>,
                       T, Ts...> : std::true_type {};

struct serialization_access {
    // Tag represents 0 or 1 types.
    template <class Device, class Format, class T, class... Tag>
    friend std::enable_if_t<has_serialize<T, Device, Format, Tag...>::value, int> serialize(
        Device& od, Format& fmt, const T& x, Tag... tag) {
        return x.serialize(od, fmt, tag...);
    }

    template <class Device, class Format, class T, class... Tag>
    friend std::enable_if_t<has_deserialize<T, Device, Format, Tag...>::value, bool> deserialize(
        Device& id, Format& fmt, T& x, Tag... tag) {
        return x.deserialize(id, fmt, tag...);
    }
};

}  // namespace ac::io
