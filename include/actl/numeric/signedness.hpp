// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <concepts>

namespace ac {

template <std::integral Int>
constexpr auto to_signed(Int x) noexcept
{
    return static_cast<std::make_signed_t<Int>>(x);
}

template <std::integral Int>
constexpr auto to_unsigned(Int x) noexcept
{
    return static_cast<std::make_unsigned_t<Int>>(x);
}

} // namespace ac
