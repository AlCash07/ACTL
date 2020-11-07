/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <tuple>

namespace ac::io {

template <class T, class = void>
struct is_manipulator : std::false_type {};

template <class T>
struct is_manipulator<T, std::void_t<typename T::is_manipulator>> : std::true_type {};

template <class Format, class Manipulator>
inline void manipulate(Format&, const Manipulator&) {}

template <class Format, class Manipulator, size_t... Is>
inline void manipulate(Format& fmt, const Manipulator& m, std::index_sequence<Is...>) {
    return (..., manipulate(std::get<Is>(fmt), m));
}

template <class... Formats, class Manipulator>
void manipulate(std::tuple<Formats...>& fmt, const Manipulator& m) {
    manipulate(fmt, m, std::make_index_sequence<sizeof...(Formats)>{});
}

}  // namespace ac::io
