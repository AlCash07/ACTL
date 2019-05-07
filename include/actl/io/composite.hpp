/***************************************************************************************************
 * I/O for standard composite types:
 * - pair
 * - tuple
 * - ranges and containers
 ***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/functions.hpp>
#include <actl/io/io.hpp>
#include <actl/traits/iterator_traits.hpp>
#include <tuple>
#include <utility>

namespace ac::io {

/* std::pair */

template <class Device, class Format, class T1, class T2>
inline index serialize(Device& od, Format& fmt, const std::pair<T1, T2>& x) {
    return write(od, fmt, x.first, x.second);
}

template <class Device, class Format, class T1, class T2>
inline bool deserialize(Device& id, Format& fmt, std::pair<T1, T2>& x) {
    // const_cast is used to be able to read std::map<Key, Value>::value_type which is
    // std::pair<const Key, Value>.
    return read(id, fmt, const_cast<std::remove_const_t<T1>&>(x.first), x.second);
}

/* std::tuple */

namespace detail {

template <class D, class F, class T, size_t... Is>
inline index writeTuple(D& od, F& fmt, const T& x, std::index_sequence<Is...>) {
    return write(od, fmt, std::get<Is>(x)...);
}

template <class D, class F, class T, size_t... Is>
inline bool readTuple(D& id, F& fmt, T& x, std::index_sequence<Is...>) {
    return read(id, fmt, std::get<Is>(x)...);
}

}  // namespace detail

template <class Device, class Format, class... Ts>
inline index serialize(Device& od, Format& fmt, const std::tuple<Ts...>& x) {
    return detail::writeTuple(od, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

template <class Device, class Format, class... Ts>
inline bool deserialize(Device& id, Format& fmt, std::tuple<Ts...>& x) {
    return detail::readTuple(id, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

/* ranges and containers */

template <class Device, class Format, class Range, class = std::enable_if_t<is_range_v<Range>>>
inline index serialize(Device& od, Format& fmt, const Range& x) {
    index res{};
    if constexpr (is_container_v<Range> && static_size_v<Range> != dynamic_size) {
        res = writeSize(od, fmt, x.size());
    }
    for (const auto& value : x) {
        res += write(od, fmt, value);
    }
    return res;
}

template <class Device, class Format, class Range, class = std::enable_if_t<is_range_v<Range>>>
inline bool deserialize(Device& id, Format& fmt, Range& x) {
    if constexpr (is_container_v<Range> && static_size_v<Range> == dynamic_size) {
        decltype(x.size()) size{};
        if (!readSize(id, fmt, size)) return false;
        if constexpr (!is_random_access_container_v<Range>) {
            for (; size > 0; --size) {
                value_type_t<Range> value;
                if (!read(id, fmt, value)) return false;
                emplace(x, std::move(value));
            }
            return true;
        } else {
            x.resize(size);
        }
    }
    for (auto& value : x) {
        if (!read(id, fmt, value)) return false;
    }
    return true;
}

}  // namespace ac::io
