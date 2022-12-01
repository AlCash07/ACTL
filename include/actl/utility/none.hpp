// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

/// Empty class to enable void instantiation and take advantage of empty-base
/// optimization.
struct none
{
    none() = default;

    // The first parameter is needed because of the bug in std::is_trivial impl.
    template <class T, class... Ts>
    explicit constexpr none(T, Ts...) noexcept
    {}
};

template <class T>
inline constexpr bool is_none_v = std::is_same_v<T, none>;

// clang-format off
template <class T> struct void_to_none             { using type = T;          };
template <>        struct void_to_none<void>       { using type = none;       };
template <>        struct void_to_none<void const> { using type = none const; };
// clang-format on

template <class T>
using void_to_none_t = typename void_to_none<T>::type;

} // namespace ac
