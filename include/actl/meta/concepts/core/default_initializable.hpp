// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// @file Extensions for the `std::default_initializable` concept
/// https://en.cppreference.com/w/cpp/concepts/default_initializable

#include <concepts>

namespace ac {

template <class T>
concept TriviallyDefaultInitializable =
    std::default_initializable<T> && std::is_trivially_constructible_v<T>;

template <class T>
concept NothrowDefaultInitializable = std::default_initializable<T> &&
                                      std::is_nothrow_constructible_v<T> &&
                                      noexcept(T{});

} // namespace ac
