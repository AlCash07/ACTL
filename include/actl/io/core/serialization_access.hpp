// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_traits.hpp>

namespace ac::io {

struct serialization_access
{
    // This is the simplest portable way I found to check for optional private
    // members in T.
    template <class T, class = typename T::is_io_tuple>
    std::true_type is_io_tuple(int);

    template <class T>
    std::false_type is_io_tuple(...);

    template <
        class T,
        class... Ts,
        class = decltype(
            std::declval<T const>().write_final(std::declval<Ts>()...))>
    std::true_type has_write(int);

    template <class... Ts>
    std::false_type has_write(...);

    template <class T, class... Ts>
    static bool write_final(T const& x, Ts&... args)
    {
        return x.write_final(args...);
    }

    template <
        class T,
        class... Ts,
        class = decltype(std::declval<T>().read_final(std::declval<Ts>()...))>
    std::true_type has_read(int);

    template <class... Ts>
    std::false_type has_read(...);

    template <class T, class... Ts>
    static bool read(T& x, Ts&&... args)
    {
        return x.read_final(args...);
    }
};

template <
    class Device,
    class Format,
    class T,
    enable_int_if<decltype(
        serialization_access{}.has_write<T, Device&, Format&>(0))::value> = 0>
bool write_final(Device& od, Format& fmt, T const& x)
{
    return serialization_access::write_final(x, od, fmt);
}

template <
    class Device,
    class Format,
    class T,
    enable_int_if<decltype(
        serialization_access{}.has_read<T, Device&, Format&>(0))::value> = 0>
bool read_final(Device& id, Format& fmt, T& x)
{
    return serialization_access::read(x, id, fmt);
}

} // namespace ac::io
