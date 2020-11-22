/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/core/apply_format.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/io/core/composite_format.hpp>
#include <actl/io/core/const_data_parser.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/parser_executor.hpp>
#include <actl/io/core/serialization_access.hpp>
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
    struct format_tag;
};

template <class Device, enable_int_if<is_bin<Device::mode>> = 0>
inline binary deduce_format(Device& dev) {
    return {};
}

template <class T, class Tag, class = void>
struct has_format_tag : std::false_type {};

template <class T, class Tag>
struct has_format_tag<T, Tag, std::void_t<typename T::format_tag>>
    : std::is_same<typename T::format_tag, Tag> {};

/* Common types support */

template <class Device, class Format, class T, enable_int_if<std::is_empty_v<T>> = 0>
inline index write_final(Device&, Format&, const T&) {
    return 0;
}

template <class Device, class Format, class T, enable_int_if<std::is_empty_v<T>> = 0>
inline bool read_final(Device&, Format&, T&) {
    return true;
}

template <class T>
using enable_int_if_byte = enable_int_if<std::is_arithmetic_v<T> && sizeof(T) == 1>;

template <class Device, class Format, class B, enable_int_if_byte<B> = 0>
inline index write_final(Device& od, Format&, B byte) {
    return od.write(static_cast<char_t<Device>>(byte));
}

template <class Device, class Format, class B, enable_int_if_byte<B> = 0>
inline bool read_final(Device& id, Format&, B& byte) {
    byte = static_cast<B>(id.get());
    return !id.eof();
}

template <class Device, class Format, class B, index N, enable_int_if_byte<B> = 0>
inline index write_final(Device& od, Format&, span<B, N> s) {
    return od.write({reinterpret_cast<const char_t<Device>*>(s.data()), s.size()});
}

template <class Device, class Format, class B, index N, enable_int_if_byte<B> = 0>
inline bool read_final(Device& id, Format&, span<B, N>& s) {
    return id.read({reinterpret_cast<char_t<Device>*>(s.data()), s.size()}) == s.size();
}

template <class Device, class Format, class B, index N, enable_int_if_byte<B> = 0>
inline bool read_final(Device& id, Format&, cspan<B, N>& s) {
    span sc{reinterpret_cast<const char*>(s.data()), s.size()};
    return parser_executor{const_data_parser{sc}}(id);
}

/* Function support */

template <class Device, class Format, class T,
          enable_int_if<std::is_invocable_r_v<bool, T&, Device&>> = 0>
inline bool read_final(Device& id, Format&, T& s) {
    return s(id);
}

/* Read and write. Absence of std::forward is intentional here to convert rvalue references into
 * lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
inline index write(Device&& od, Format&& fmt, const Ts&... args) {
    if constexpr (is_format_v<Format>) {
        return (... + detail::write_impl(od, fmt, fmt, args));
    } else {
        return write(od, deduce_format(od), fmt, args...);
    }
}

template <class Device, class Format, class... Ts>
inline bool read(Device&& id, Format&& fmt, Ts&&... args) {
    if constexpr (is_format_v<Format>) {
        return (... && detail::read_impl(id, fmt, fmt, args));
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

}  // namespace ac::io
