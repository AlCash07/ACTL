// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// @file Extensions for the `std::copy_constructible` concept
/// https://en.cppreference.com/w/cpp/concepts/copy_constructible

#include <actl/meta/concepts/core/constructible_from.hpp>
#include <actl/meta/concepts/core/convertible_to.hpp>

namespace ac {

template <class T>
concept TriviallyCopyConstructible =
    std::copy_constructible<T> &&
    // TODO: comment why different flawors of copy are necessary.
    TriviallyConstructibleFrom<T, T&> &&
    TriviallyConstructibleFrom<T, T const&> &&
    TriviallyConstructibleFrom<T, T const>;
// Notion of trivial conversion doesn't exist, so we don't check it.

template <class T>
concept NothrowCopyConstructible =
    std::copy_constructible<T> &&
    // TODO: comment why both constructible and convertible are checked.
    NothrowConstructibleFrom<T, T&> && NothrowConvertibleTo<T&, T> &&
    NothrowConstructibleFrom<T, T const&> &&
    NothrowConvertibleTo<T const&, T> && NothrowConstructibleFrom<T, T const> &&
    NothrowConvertibleTo<T const, T>;

template <class T>
// A type is broken if some copy constructor versions may throw and some not.
concept MayThrowCopyConstructible = std::copy_constructible<T> && !
std::is_nothrow_constructible_v<T, T&>&& MayThrowConvertibleTo<T&, T> &&
    !std::is_nothrow_constructible_v<T, T const&> &&
    MayThrowConvertibleTo<T const&, T> &&
    !std::is_nothrow_constructible_v<T, T const> &&
    MayThrowConvertibleTo<T const, T>;

} // namespace ac
