/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/type_traits.hpp>
#include <actl/types.hpp>

namespace ac::io {

struct serialization_access {
    // This is the simplest portable way I found to check for optional private members in T.
    template <class T, class = typename T::composite_io_tag>
    std::true_type has_composite_io_tag(int);

    template <class T>
    std::false_type has_composite_io_tag(...);

    template <class T, class... Ts,
              class = decltype(std::declval<const T>().serialize(std::declval<Ts>()...))>
    std::true_type has_serialize(int);

    template <class... Ts>
    std::false_type has_serialize(...);

    template <class T, class... Ts>
    static index write(const T& x, Ts&&... args) {
        return x.serialize(args...);
    }

    template <class T, class... Ts,
              class = decltype(std::declval<T>().deserialize(std::declval<Ts>()...))>
    std::true_type has_deserialize(int);

    template <class... Ts>
    std::false_type has_deserialize(...);

    template <class T, class... Ts>
    static bool read(T& x, Ts&&... args) {
        return x.deserialize(args...);
    }
};

// Tag represents 0 or 1 types.
template <class Device, class Format, class T, class... Tag,
          enable_int_if<decltype(
              serialization_access{}.has_serialize<T, Device&, Format&, Tag...>(0))::value> = 0>
inline index serialize(Device& od, Format& fmt, const T& x, Tag... tag) {
    return serialization_access::write(x, od, fmt, tag...);
}

template <class Device, class Format, class T, class... Tag,
          enable_int_if<decltype(
              serialization_access{}.has_deserialize<T, Device&, Format&, Tag...>(0))::value> = 0>
inline bool deserialize(Device& id, Format& fmt, T& x, Tag... tag) {
    return serialization_access::read(x, id, fmt, tag...);
}

}  // namespace ac::io
