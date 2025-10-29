// https://en.cppreference.com/w/cpp/concepts/copyable
//
// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/concepts/core/copy_constructible.hpp>
#include <actl/meta/concepts/object/movable.hpp>

namespace ac {

template<typename T>
concept TriviallyCopyable =
    std::copyable<T> && TriviallyCopyConstructible<T> && TriviallyMovable<T> &&
    TriviallyAssignableFrom<T&, T&> && TriviallyAssignableFrom<T&, const T&> &&
    TriviallyAssignableFrom<T&, const T>;

template<typename T>
concept NothrowCopyable =
    std::copyable<T> && NothrowCopyConstructible<T> && NothrowMovable<T> &&
    NothrowAssignableFrom<T&, T&> && NothrowAssignableFrom<T&, const T&> &&
    NothrowAssignableFrom<T&, const T>;

template<typename T>
// A type is broken if some copy versions may throw and some not.
concept MayThrowCopyable = std::copyable<T> && MayThrowCopyConstructible<T> &&
                           !std::is_nothrow_assignable_v<T&, T&> &&
                           !std::is_nothrow_assignable_v<T&, const T&> &&
                           !std::is_nothrow_assignable_v<T&, const T>;

} // namespace ac
