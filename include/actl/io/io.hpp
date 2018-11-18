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
inline constexpr uint8_t bin = 0x01;
inline constexpr uint8_t in  = 0x02;
inline constexpr uint8_t out = 0x04;
inline constexpr uint8_t app = 0x08;
inline constexpr uint8_t own = 0x10;
inline constexpr uint8_t line_buffered = 0x20;

struct empty {};

template <mode_t Mode>
using base_t = std::conditional_t<0 < (Mode & bin), empty, formatter>;

}  // namespace ac::io
