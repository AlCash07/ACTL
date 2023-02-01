// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// @file Extensions for the `std::constructible_from` concept
/// https://en.cppreference.com/w/cpp/concepts/constructible_from

#include <concepts>

namespace ac {

template <class T, class... Args>
concept TriviallyConstructibleFrom =
    std::constructible_from<T, Args...> &&
    std::is_trivially_destructible_v<T> &&
    std::is_trivially_constructible_v<T, Args...>;

template <class T, class... Args>
concept NothrowConstructibleFrom =
    std::constructible_from<T, Args...> &&
    // `std::constructible_from` subsumes `std::destructible` which requires the
    // destructor to not throw, so we don't have to check that here, see
    // https://en.cppreference.com/w/cpp/concepts/destructible
    std::is_nothrow_constructible_v<T, Args...>;

} // namespace ac
