// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/manipulator.hpp>
#include <actl/meta/tuple.hpp>
#include <actl/utility/index.hpp>

namespace ac::io {

namespace detail {

template <class D, class F, class T, size_t... Is>
bool write_tuple(D& od, F& fmt, T const& x, std::index_sequence<Is...>)
{
    using std::get;
    return write(od, fmt, get<Is>(x)...);
}

template <class D, class F, class T, size_t... Is>
bool read_tuple(D& id, F& fmt, T& x, std::index_sequence<Is...>)
{
    using std::get;
    return read(id, fmt, get<Is>(x)...);
}

} // namespace detail

template <Tuple T>
bool write_final(Device auto& od, Format auto& fmt, T const& x)
{
    nested_scope_guard g{fmt};
    return detail::write_tuple(od, fmt, x, tuple_indices_t<T>{});
}

template <Tuple T>
bool read_final(Device auto& id, Format auto& fmt, T& x)
{
    nested_scope_guard g{fmt};
    return detail::read_tuple(id, fmt, x, tuple_indices_t<T>{});
}

} // namespace ac::io
