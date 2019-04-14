/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstdint>
#include <type_traits>

namespace ac::io {

struct format {};

struct binary : format {};
struct text : format {};

template <class T>
inline constexpr bool is_format_v = std::is_base_of_v<format, T>;

using mode_t = uint8_t;
inline constexpr mode_t bin = 0x01;
inline constexpr mode_t in  = 0x02;
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

struct device {};

template <mode_t Mode>
struct base : device {
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    static constexpr mode_t mode = Mode;
};

template <class T>
inline constexpr bool is_device_v = std::is_base_of_v<device, T>;

// TODO: when concepts are available, call this by default if the second parameter isn't a format.
template <class Device>
inline auto deduce_format(Device& device) {
    if constexpr (is_bin<Device::mode>) {
        return binary{};
    } else {
        return device.format();
    }
}

}  // namespace ac::io
