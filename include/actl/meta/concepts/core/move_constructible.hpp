// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/concepts/core/constructible_from.hpp>
#include <actl/meta/concepts/core/convertible_to.hpp>

namespace ac {

template <class T>
concept TriviallyMoveConstructible =
    std::move_constructible<T> && TriviallyConstructibleFrom<T, T>;
// Notion of trivial conversion doesn't exist, so we don't check it.

template <class T>
concept NothrowMoveConstructible =
    std::move_constructible<T> && NothrowConstructibleFrom<T, T> &&
    NothrowConvertibleTo<T, T>;

} // namespace ac
