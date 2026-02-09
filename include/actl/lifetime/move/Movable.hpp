// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/lifetime/assignment/AssignableFrom.hpp>
#include <actl/lifetime/move/MoveConstructible.hpp>
#include <actl/lifetime/swap/Swappable.hpp>

namespace ac {

template<typename T>
concept TriviallyMovable = std::movable<T> && TriviallyMoveConstructible<T> &&
                           TriviallyAssignableFrom<T&, T>;
// Notion of trivial swap doesn't exist, so we don't check it.

template<typename T>
concept NothrowMovable = std::movable<T> && NothrowMoveConstructible<T> &&
                         NothrowAssignableFrom<T&, T> && NothrowSwappable<T>;

} // namespace ac
