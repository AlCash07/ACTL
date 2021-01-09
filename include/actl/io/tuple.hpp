// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/tuple.hpp>
#include <actl/io/core/manipulator.hpp>

namespace ac::io {

namespace detail {

template <class D, class F, class T, size_t... Is>
index write_tuple(D& od, F& fmt, const T& x, std::index_sequence<Is...>) {
    using std::get;
    return write(od, fmt, get<Is>(x)...);
}

template <class D, class F, class T, size_t... Is>
bool read_tuple(D& id, F& fmt, T& x, std::index_sequence<Is...>) {
    using std::get;
    return read(id, fmt, get<Is>(x)...);
}

}  // namespace detail

template <class Device, class Format, class T, enable_int_if<is_tuple_v<T>> = 0>
index write_final(Device& od, Format& fmt, const T& x) {
    nested_scope_guard g{fmt};
    return detail::write_tuple(od, fmt, x, std::make_index_sequence<std::tuple_size_v<T>>{});
}

template <class Device, class Format, class T, enable_int_if<is_tuple_v<T>> = 0>
bool read_final(Device& id, Format& fmt, T& x) {
    nested_scope_guard g{fmt};
    return detail::read_tuple(id, fmt, x, std::make_index_sequence<std::tuple_size_v<T>>{});
}

}  // namespace ac::io
