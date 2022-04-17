// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/byte.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/serialization_access.hpp>
#include <actl/io/device/traits.hpp>
#include <actl/io/format/composed_format.hpp>
#include <actl/meta/type_traits.hpp>
#include <cstdint>

namespace ac::io {

/* Device */

using mode_t = uint8_t;
inline constexpr mode_t bin = 0x01;
inline constexpr mode_t in = 0x02;
inline constexpr mode_t out = 0x04;
inline constexpr mode_t app = 0x08;
inline constexpr mode_t trunc = in | out | app;

template <mode_t Mode>
inline constexpr bool is_bin = (Mode & bin) > 0;

template <mode_t Mode>
inline constexpr bool is_in = (Mode & in) > 0;

template <mode_t Mode>
inline constexpr bool is_out = (Mode & (out | app)) > 0;

template <mode_t Mode, class Char>
struct device : device_base
{
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    using char_type = Char;

    static constexpr mode_t mode = Mode;
};

/* Format */

struct binary
{
    struct format_tag;
};

template <class Device, enable_int_if<is_bin<Device::mode>> = 0>
binary deduce_format(Device&)
{
    return {};
}

template <class T, class Tag, class = void>
struct has_format_tag : std::false_type
{};

template <class T, class Tag>
struct has_format_tag<T, Tag, std::void_t<typename T::format_tag>>
    : std::is_same<typename T::format_tag, Tag>
{};

/* Read and write. Absence of std::forward is intentional here to convert rvalue
   references into
   lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
bool write(Device&& od, Format&& fmt, const Ts&... args)
{
    if constexpr (is_format_v<Format>)
    {
        return (... && detail::write_impl(od, fmt, fmt, args));
    }
    else
    {
        return write(od, deduce_format(od), fmt, args...);
    }
}

template <class Device, class Format, class... Ts>
bool read(Device&& id, Format&& fmt, Ts&&... args)
{
    if constexpr (is_format_v<Format>)
    {
        return (... && detail::read_impl(id, fmt, fmt, args));
    }
    else
    {
        return read(id, deduce_format(id), fmt, args...);
    }
}

template <class Device, class Format, class T>
bool write_size(Device& od, Format& fmt, const T& size)
{
    return write(od, fmt, size);
}

template <class Device, class Format, class T>
bool read_size(Device& id, Format& fmt, T& size)
{
    return read(id, fmt, size);
}

} // namespace ac::io
