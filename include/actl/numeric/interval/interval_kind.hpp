// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace ac {

enum class endpoint_kind : uint8_t
{
    open = 0,
    closed = 1,
    unbounded = 2,
};

inline constexpr uint8_t combine(
    endpoint_kind begin, endpoint_kind end) noexcept
{
    return static_cast<uint8_t>(
        static_cast<int>(begin) << 2 | static_cast<int>(end));
}

// clang-format off
enum class interval_kind : uint8_t {
    open             = combine(endpoint_kind::open, endpoint_kind::open),
    open_closed      = combine(endpoint_kind::open, endpoint_kind::closed),
    open_unbounded   = combine(endpoint_kind::open, endpoint_kind::unbounded),
    closed_open      = combine(endpoint_kind::closed, endpoint_kind::open),
    closed           = combine(endpoint_kind::closed, endpoint_kind::closed),
    closed_unbounded = combine(endpoint_kind::closed, endpoint_kind::unbounded),
    unbounded_open   = combine(endpoint_kind::unbounded, endpoint_kind::open),
    unbounded_closed = combine(endpoint_kind::unbounded, endpoint_kind::closed),
    unbounded        = combine(endpoint_kind::unbounded, endpoint_kind::unbounded),
};
// clang-format on

inline constexpr endpoint_kind interval_begin(interval_kind kind) noexcept
{
    return static_cast<endpoint_kind>(static_cast<int>(kind) >> 2);
}

inline constexpr endpoint_kind interval_end(interval_kind kind) noexcept
{
    return static_cast<endpoint_kind>(static_cast<int>(kind) & 3);
}

} // namespace ac
