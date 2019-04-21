/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>
#include <actl/util/none.hpp>
#include <cstdint>
#include <type_traits>

namespace ac::io {

/* Device */

using mode_t = uint8_t;
inline constexpr mode_t bin = 0x01;
inline constexpr mode_t in = 0x02;
inline constexpr mode_t out = 0x04;
inline constexpr mode_t app = 0x08;
inline constexpr mode_t line_buffered = 0x10;
inline constexpr mode_t trunc = in | out | app;

template <mode_t Mode>
inline constexpr bool is_in = (Mode & in) > 0;

template <mode_t Mode>
inline constexpr bool is_out = (Mode & (out | app)) > 0;

template <mode_t Mode>
inline constexpr bool is_line_buffered = (Mode & line_buffered) > 0;

struct device {};

template <mode_t Mode>
struct base : device {
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    static constexpr mode_t mode = Mode;
};

template <class T>
inline constexpr bool is_device_v = std::is_base_of_v<device, T>;

/* Format */

struct format {};

struct binary {};
struct text {};

template <class T>
struct format_traits {
    using tag = none;
};

template <>
struct format_traits<binary> {
    using base = format;
    using tag = binary;
};

template <>
struct format_traits<text> {
    using base = format;
    using tag = text;
};

template <class T>
using format_tag_t = typename format_traits<T>::tag;

template <class Device>
inline auto deduce_format(Device& device) {
    if constexpr ((Device::mode & bin) > 0) {
        return binary{};
    } else {
        return device.format();
    }
}

/* Default serialization forwarding */

template <class Device, class Format, class T>
inline int serialize(Device& od, Format& fmt, T&& x, format) {
    return serialize(od, fmt, x);
}

template <class Device, class Format, class T>
inline bool deserialize(Device& id, Format& fmt, T&& x, format) {
    return deserialize(id, fmt, x);
}

template <class Device, class Format, class T, class Tag>
inline int serialize(Device& od, Format& fmt, T&& x, Tag) {
    return serialize(od, fmt, x, typename format_traits<Tag>::base{});
}

template <class Device, class Format, class T, class Tag>
inline bool deserialize(Device& id, Format& fmt, T&& x, Tag) {
    return deserialize(id, fmt, x, typename format_traits<Tag>::base{});
}

/* Read and write. Absence of std::forward is intentional here to convert rvalue references into
 * lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
inline int write(Device&& od, Format&& fmt, Ts&&... args) {
    if constexpr (std::is_same_v<format_tag_t<std::remove_cv_t<Format>>, none>) {
        return write(od, deduce_format(od), fmt, args...);
    } else {
        return (... + serialize(od, fmt, args, format_tag_t<Format>{}));
    }
}

template <class Device, class Format, class... Ts>
inline bool read(Device&& id, Format&& fmt, Ts&&... args) {
    if constexpr (std::is_same_v<format_tag_t<std::remove_cv_t<Format>>, none>) {
        return read(id, deduce_format(id), fmt, args...);
    } else {
        return (... && deserialize(id, fmt, args, format_tag_t<Format>{}));
    }
}

template <class... Ts>
inline int writeln(Ts&&... args) {
    return write(args..., '\n');
}

template <class Device, class Format, class T>
inline int writeSize(Device& od, Format& fmt, const T& size) {
    return write(od, fmt, size);
}

template <class Device, class Format, class T>
inline bool readSize(Device& od, Format& fmt, T& size) {
    return read(od, fmt, size);
}

}  // namespace ac::io
