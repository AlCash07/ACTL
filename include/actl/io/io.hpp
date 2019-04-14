/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>
#include <actl/io/util/serialization_access.hpp>
#include <actl/util/use_default.hpp>

namespace ac::io {

/* Composite types  */

template <class T>
struct is_composite : has_serialization<T> {};

template <class T>
inline constexpr bool is_composite_v = is_composite<T>::value;

/* Convert rvalue references to lvalue references for the duration of the function call */

template <class Device, class Format, class... Ts>
inline std::enable_if_t<!std::is_reference_v<Device> || !std::is_reference_v<Format>, int> write(
    Device&& od, Format&& fmt, const Ts&... args) {
    return write(od, fmt, args...);
}

template <class Device, class Format, class... Ts>
inline std::enable_if_t<!std::is_reference_v<Device> || !std::is_reference_v<Format>, bool> read(
    Device&& id, Format&& fmt, Ts&&... args) {
    return read(id, fmt, std::forward<Ts>(args)...);
}

/* Generic forwarding */

template <class Device, class Format, class T, class... Ts>
inline int write(Device& od, Format& fmt, const T& x, const Ts&... args) {
    return write(od, fmt, x) + write(od, fmt, args...);
}

template <class Device, class Format, class T, class... Ts>
inline bool read(Device& id, Format& fmt, T&& x, Ts&&... args) {
    return read(id, fmt, std::forward<T>(x)) && read(id, fmt, std::forward<Ts>(args)...);
}

template <class Device, class... Ts>
inline int writeln(Device&& od, Ts&&... args) {
    return write(od, std::forward<Ts>(args)..., '\n');
}

}  // namespace ac::io
