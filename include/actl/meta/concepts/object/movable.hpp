// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// @file Extensions for the `std::movable` concept
/// https://en.cppreference.com/w/cpp/concepts/movable

#include <actl/meta/concepts/core/assignable_from.hpp>
#include <actl/meta/concepts/core/move_constructible.hpp>
#include <actl/meta/concepts/core/swappable.hpp>

namespace ac {

template <class T>
concept TriviallyMovable = std::movable<T> && TriviallyMoveConstructible<T> &&
                           TriviallyAssignableFrom<T&, T>;
// Notion of trivial swap doesn't exist, so we don't check it.

template <class T>
concept NothrowMovable = std::movable<T> && NothrowMoveConstructible<T> &&
                         NothrowAssignableFrom<T&, T> && NothrowSwappable<T>;

} // namespace ac
