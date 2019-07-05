/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>
#include <actl/util/none.hpp>
#include <actl/util/span.hpp>
#include <actl/util/type_traits.hpp>
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

template <class T, class = void>
struct has_input_buffer : std::false_type {};

template <class T>
struct has_input_buffer<T, std::void_t<decltype(std::declval<T>().input_data())>> : std::true_type {
};

template <class T, class = void>
struct has_output_buffer : std::false_type {};

template <class T>
struct has_output_buffer<T, std::void_t<decltype(std::declval<T>().output_data())>>
    : std::true_type {};

/* Format */

struct binary {
    using format_tag = binary;
    using base = none;
};

struct text_tag {
    using base = none;
};

template <class T, class = void>
struct is_format : std::false_type {};

template <class T>
struct is_format<T, std::void_t<typename T::format_tag>> : std::true_type {};

template <class Device>
inline decltype(auto) deduce_format(Device& dev) {
    if constexpr (is_bin<Device::mode>) {
        return binary{};
    } else {
        return dev.format();
    }
}

/* Common types support */

template <class Device, class Format, class T, enable_int_if<std::is_empty_v<T>> = 0>
inline index serialize(Device&, Format&, const T&) {
    return 0;
}

template <class Device, class Format, class T, enable_int_if<std::is_empty_v<T>> = 0>
inline bool deserialize(Device&, Format&, T&) {
    return true;
}

template <class Device, class Format>
inline index serialize(Device& od, Format&, char_t<Device> c) {
    return od.write(c);
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format&, char_t<Device>& c) {
    c = id.get();
    return !id.eof();
}

template <class Device, class Format, index N>
inline index serialize(Device& od, Format&, const span<char_t<Device>, N>& s) {
    return od.write(s);
}

template <class Device, class Format, index N>
inline index serialize(Device& od, Format&, const cspan<char_t<Device>, N>& s) {
    return od.write(s);
}

// TODO: add overloads taking const span&.
template <class Device, class Format, index N>
inline bool deserialize(Device& id, Format&, span<char_t<Device>, N>& s) {
    return id.read(s) == s.size();
}

template <class Device, class Format, index N>
inline bool deserialize(Device& id, Format&, cspan<char_t<Device>, N>& s) {
    for (char c : s) {
        if (id.peek() != c) return false;
        id.move(1);
    }
    return true;
}

/* Default forwarding */

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
    using F = std::remove_reference_t<Format>;
    if constexpr (is_format<F>::value) {
        return (... + serialize(od, fmt, args, typename F::format_tag{}));
    } else {
        return write(od, deduce_format(od), fmt, args...);
    }
}

template <class Device, class Format, class... Ts>
inline bool read(Device&& id, Format&& fmt, Ts&&... args) {
    using F = std::remove_reference_t<Format>;
    if constexpr (is_format<F>::value) {
        return (... && deserialize(id, fmt, args, typename F::format_tag{}));
    } else {
        return read(id, deduce_format(id), fmt, args...);
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
struct is_composite : decltype(serialization_access{}.has_composite_io_tag<T>(0)) {};

template <class T>
struct is_manipulator : std::false_type {};

}  // namespace ac::io
