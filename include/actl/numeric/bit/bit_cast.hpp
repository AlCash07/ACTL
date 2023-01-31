// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstring>

namespace ac {

// C++20 version requires std::is_trivially_constructible_v<To>.
template <class To, class From>
    requires(
        sizeof(To) == sizeof(From) && std::is_default_constructible_v<To> &&
        std::is_trivially_copyable_v<To> && std::is_trivially_copyable_v<From>)
To bit_cast(From const& src) noexcept
{
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

} // namespace ac
