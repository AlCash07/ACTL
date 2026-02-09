// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

/// Checks if the type has both `const` and `volatile` qualifiers.
template<typename T>
inline constexpr bool is_cv_v = std::is_const_v<T> && std::is_volatile_v<T>;

/// Checks if the type has any of `const`, `volatile` or reference qualifiers.
template<typename T>
inline constexpr bool has_qualifiers_v =
    std::is_reference_v<T> || std::is_const_v<T> || std::is_volatile_v<T>;

} // namespace ac
