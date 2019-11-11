/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>
#include <actl/util/type_traits.hpp>

namespace ac::io {

struct serialization_access {
    // This is the simplest portable way I found to check for optional private members in T.
    template <class T, class = typename T::io_tuple_tag>
    std::true_type has_io_tuple_tag(int);

    template <class T>
    std::false_type has_io_tuple_tag(...);

    template <class T, class... Ts,
              class = decltype(std::declval<const T>().write_final(std::declval<Ts>()...))>
    std::true_type has_write(int);

    template <class... Ts>
    std::false_type has_write(...);

    template <class T, class... Ts>
    static index write_final(const T& x, Ts&... args) {
        return x.write_final(args...);
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

template <
    class Device, class Format, class T,
    enable_int_if<decltype(serialization_access{}.has_write<T, Device&, Format&>(0))::value> = 0>
inline index write_final(Device& od, Format& fmt, const T& x) {
    return serialization_access::write_final(x, od, fmt);
}

template <class Device, class Format, class T, class... Tag,
          enable_int_if<decltype(
              serialization_access{}.has_deserialize<T, Device&, Format&, Tag...>(0))::value> = 0>
inline bool deserialize(Device& id, Format& fmt, T& x, Tag... tag) {
    return serialization_access::read(x, id, fmt, tag...);
}

}  // namespace ac::io
