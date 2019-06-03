/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>
#include <actl/traits/type_traits.hpp>
#include <actl/util/none.hpp>
#include <actl/util/span.hpp>
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
using default_char_t = char;
// TODO: uncomment when wchar and std::byte support is added.
// using default_char_t = std::conditional_t<is_bin<Mode>, std::byte, char>;

template <class Device>
using char_t = typename Device::char_type;

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

struct binary {
    using base = none;
};

struct text_tag {
    using base = none;
};

template <class T>
struct format_traits {
    using tag = none;
};

template <>
struct format_traits<binary> {
    using tag = binary;
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
inline index serialize(Device& od, Format&, char_t<Device> c) {
    return od.put(c);
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format&, char_t<Device>& c) {
    c = id.get();
    return !id.eof();
}

template <class Device, class Format>
inline index serialize(Device& od, Format&, const cspan<char_t<Device>>& s) {
    return od.write(s);
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format&, const span<char_t<Device>>& s) {
    return id.read(s) == s.size();
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format&, const cspan<char_t<Device>>& s) {
    for (char c : s) {
        if (id.peek() != c) return false;
        id.move(1);
    }
    return true;
}

template <class Device, class Format, class T>
inline index serialize(Device& od, Format& fmt, const T& x, none) {
    return serialize(od, fmt, x);
}

template <class Device, class Format, class T>
inline bool deserialize(Device& id, Format& fmt, T& x, none) {
    return deserialize(id, fmt, x);
}

template <class Device, class Format, class T, class Tag>
inline index serialize(Device& od, Format& fmt, const T& x, Tag) {
    return serialize(od, fmt, x, typename Tag::base{});
}

template <class Device, class Format, class T, class Tag>
inline bool deserialize(Device& id, Format& fmt, T& x, Tag) {
    return deserialize(id, fmt, x, typename Tag::base{});
}

/* Read and write. Absence of std::forward is intentional here to convert rvalue references into
 * lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
inline index write(Device&& od, Format&& fmt, const Ts&... args) {
    using tag = format_tag_t<Format>;
    if constexpr (std::is_same_v<tag, none>) {
        return write(od, deduce_format(od), fmt, args...);
    } else {
        return (... + serialize(od, fmt, args, tag{}));
    }
}

template <class Device, class Format, class... Ts>
inline bool read(Device&& id, Format&& fmt, Ts&&... args) {
    using tag = format_tag_t<Format>;
    if constexpr (std::is_same_v<tag, none>) {
        return read(id, deduce_format(id), fmt, args...);
    } else {
        return (... && deserialize(id, fmt, args, tag{}));
    }
}

template <class Device, class Format, class T>
inline index write_size(Device& od, Format& fmt, const T& size) {
    return write(od, fmt, size);
}

template <class Device, class Format, class T>
inline bool read_size(Device& id, Format& fmt, T& size) {
    return read(id, fmt, size);
}

/* Traits */

template <class T>
struct is_composite : decltype(serialization_access::has_composite_io_tag<T>(0)) {};

template <class T>
struct is_manipulator : std::false_type {};

}  // namespace ac::io
