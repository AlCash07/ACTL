// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstring>
#include <type_traits>

namespace ac {

// C++20 version requires std::is_trivially_constructible_v<Target>.
template<typename Target, typename Source>
    requires(sizeof(Target) == sizeof(Source) && std::is_default_constructible_v<Target> && std::is_trivially_copyable_v<Target> && std::is_trivially_copyable_v<Source>)
Target bit_cast(Source const& source) noexcept {
    Target output;
    std::memcpy(&output, &source, sizeof(Target));
    return output;
}

} // namespace ac
