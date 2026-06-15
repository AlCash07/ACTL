// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/manipulator.hpp>
#include <actl/sequence/tuple/TupleConcept.hpp>
#include <actl/utility/index.hpp>

namespace ac::io {

namespace detail {

template<typename D, typename F, typename T, size_t... Is>
bool write_tuple(D& od, F& fmt, T const& tuple, std::index_sequence<Is...>) {
    using std::get;
    return write(od, fmt, get<Is>(tuple)...);
}

template<typename D, typename F, typename T, size_t... Is>
bool read_tuple(D& id, F& fmt, T& tuple, std::index_sequence<Is...>) {
    using std::get;
    return read(id, fmt, get<Is>(tuple)...);
}

} // namespace detail

template<Tuple T>
bool write_final(Device auto& od, Format auto& fmt, T const& tuple) {
    nested_scope_guard guard{fmt};
    return detail::write_tuple(od, fmt, tuple, tuple_indices_t<T>{});
}

template<Tuple T>
bool read_final(Device auto& id, Format auto& fmt, T& tuple) {
    nested_scope_guard guard{fmt};
    return detail::read_tuple(id, fmt, tuple, tuple_indices_t<T>{});
}

} // namespace ac::io
