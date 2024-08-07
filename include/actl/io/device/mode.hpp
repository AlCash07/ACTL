// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace ac::io {

using mode_t = uint8_t;
inline constexpr mode_t bin = 0x01;
inline constexpr mode_t in = 0x02;
inline constexpr mode_t out = 0x04;
inline constexpr mode_t app = 0x08;
inline constexpr mode_t trunc = in | out | app;

template<mode_t Mode>
inline constexpr bool is_bin = (Mode & bin) > 0;

template<mode_t Mode>
inline constexpr bool is_in = (Mode & in) > 0;

template<mode_t Mode>
inline constexpr bool is_out = (Mode & (out | app)) > 0;

} // namespace ac::io
