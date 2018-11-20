/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/formatter.hpp>
#include <type_traits>

namespace ac::io {

using mode_t = uint8_t;
inline constexpr mode_t bin = 0x01;
inline constexpr mode_t in  = 0x02;
inline constexpr mode_t out = 0x04;
inline constexpr mode_t app = 0x08;
inline constexpr mode_t line_buffered = 0x10;
inline constexpr mode_t trunc = in | out | app;

template <mode_t Mode>
inline constexpr bool in_v = (Mode & in) > 0;

template <mode_t Mode>
inline constexpr bool out_v = (Mode & (out | app)) > 0;

template <mode_t Mode>
inline constexpr bool line_buffered_v = (Mode & line_buffered) > 0;

struct empty {};

template <mode_t Mode>
struct base : std::conditional_t<0 < (Mode & bin), empty, formatter> {
    static_assert(in_v<Mode> || out_v<Mode>, "invalid mode");

    static constexpr mode_t mode = Mode;
};

}  // namespace ac::io
