/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>
#include <actl/util/use_default.hpp>

namespace ac::io {

/* Convert rvalue references to lvalue references for the duration of the function call */

template <class Format, class Device, class... Ts>
inline std::enable_if_t<!std::is_reference_v<Format> || !std::is_reference_v<Device>, int> write(
    Format&& fmt, Device&& od, const Ts&... args) {
    return write(fmt, od, args...);
}

template <class Format, class Device, class... Ts>
inline std::enable_if_t<!std::is_reference_v<Format> || !std::is_reference_v<Device>, bool> read(
    Format&& fmt, Device&& id, Ts&&... args) {
    return read(fmt, id, std::forward<Ts>(args)...);
}

/* Format deduction */

template <class Format, class Device>
inline auto deduce_format(Device& device) {
    if constexpr (std::is_same_v<Format, use_default>) {
        if constexpr (is_bin<Device::mode>) {
            return binary{};
        } else {
            return device.format();
        }
    } else {
        return Format{};
    }
}

template <class Format = use_default, class Device, class... Ts>
inline int write(Device& od, const Ts&... args) {
    return write(deduce_format<Format>(od), od, args...);
}

template <class Format = use_default, class Device, class... Ts>
inline bool read(Device& id, Ts&&... args) {
    return read(deduce_format<Format>(id), id, std::forward<Ts>(args)...);
}

/* Generic forwarding */

template <class Format, class Device, class T, class... Ts>
inline int write(Format& fmt, Device& od, const T& x, const Ts&... args) {
    return write(fmt, od, x) + write(fmt, od, args...);
}

template <class Format, class Device, class T, class... Ts>
inline bool read(Format& fmt, Device& id, T&& x, Ts&&... args) {
    return read(fmt, id, std::forward<T>(x)) && read(fmt, id, std::forward<Ts>(args)...);
}

template <class Format = use_default, class Device, class... Ts>
inline int writeln(Device&& od, const Ts&... args) {
    return write<Format>(od, args..., '\n');
}

template <class Format, class Device, class... Ts>
inline int writeln(Format&& fmt, Device&& od, const Ts&... args) {
    return write(fmt, od, args..., '\n');
}

}  // namespace ac::io
