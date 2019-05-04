/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>
#include <actl/types.hpp>
#include <actl/util/none.hpp>
#include <actl/util/span.hpp>
#include <actl/traits/type_traits.hpp>
#include <cstdint>

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
inline constexpr bool is_bin = (Mode & bin) > 0;

template <mode_t Mode>
inline constexpr bool is_in = (Mode & in) > 0;

template <mode_t Mode>
inline constexpr bool is_out = (Mode & (out | app)) > 0;

template <mode_t Mode>
inline constexpr bool is_line_buffered = (Mode & line_buffered) > 0;

template <mode_t Mode>
using char_t = char;
// TODO: uncomment when wchar and std::byte support is added.
// using char_t = std::conditional_t<is_bin<Mode>, std::byte, char>;

struct device_base {};

template <mode_t Mode, class Char>
struct device : device_base {
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    using char_type = Char;

    static constexpr mode_t mode = Mode;
};

template <class T>
inline constexpr bool is_device_v = std::is_base_of_v<device_base, T>;

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
using format_tag_t = typename format_traits<remove_cvref_t<T>>::tag;

template <class Device>
inline auto deduce_format(Device& dev) {
    if constexpr (is_bin<Device::mode>) {
        return binary{};
    } else {
        return dev.format();
    }
}

/* Default serialization forwarding */

template <class Device, class Format>
inline index serialize(Device& id, Format&, typename Device::char_type c) {
    return static_cast<int>(id.put(c));
}

template <class Device, class Format>
inline bool deserialize(Device& od, Format&, typename Device::char_type& c) {
    c = od.get();
    return !od.eof();
}

template <class Device, class Format>
inline index serialize(Device& id, Format&, span<const typename Device::char_type> s) {
    return id.write(s);
}

template <class Device, class Format, index N>
inline index serialize(Device& id, Format& fmt, const typename Device::char_type (&array)[N]) {
    return serialize(id, fmt, span{array, array[N - 1] ? N : N - 1});
}

template <class Device, class Format>
inline bool deserialize(Device& od, Format&, span<typename Device::char_type> s) {
    return od.read(s) == s.size();
}

template <class Device, class Format, class T>
inline index serialize(Device& od, Format& fmt, T& x, format) {
    return serialize(od, fmt, x);
}

template <class Device, class Format, class T>
inline bool deserialize(Device& id, Format& fmt, T& x, format) {
    return deserialize(id, fmt, x);
}

template <class Device, class Format, class T, class Tag>
inline index serialize(Device& od, Format& fmt, T& x, Tag) {
    return serialize(od, fmt, x, typename format_traits<Tag>::base{});
}

template <class Device, class Format, class T, class Tag>
inline bool deserialize(Device& id, Format& fmt, T& x, Tag) {
    return deserialize(id, fmt, x, typename format_traits<Tag>::base{});
}

/* Read and write. Absence of std::forward is intentional here to convert rvalue references into
 * lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
inline index write(Device&& od, Format&& fmt, Ts&&... args) {
    if constexpr (std::is_same_v<format_tag_t<Format>, none>) {
        return write(od, deduce_format(od), fmt, args...);
    } else {
        return (... + serialize(od, fmt, args, format_tag_t<Format>{}));
    }
}

template <class Device, class Format, class... Ts>
inline bool read(Device&& id, Format&& fmt, Ts&&... args) {
    if constexpr (std::is_same_v<format_tag_t<Format>, none>) {
        return read(id, deduce_format(id), fmt, args...);
    } else {
        return (... && deserialize(id, fmt, args, format_tag_t<Format>{}));
    }
}

template <class... Ts>
inline index writeln(Ts&&... args) {
    return write(args..., '\n');
}

template <class Device, class Format, class T>
inline index writeSize(Device& od, Format& fmt, const T& size) {
    return write(od, fmt, size);
}

template <class Device, class Format, class T>
inline bool readSize(Device& od, Format& fmt, T& size) {
    return read(od, fmt, size);
}

}  // namespace ac::io
