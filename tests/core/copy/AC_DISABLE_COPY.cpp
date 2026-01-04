// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/core/copy/AC_DISABLE_COPY.hpp>
#include <concepts>

struct no_copy {
    AC_DISABLE_COPY(no_copy)
};

static_assert(!std::is_copy_constructible_v<no_copy>);
static_assert(!std::is_copy_assignable_v<no_copy>);
/* move and default constructors are also disabled */
static_assert(!std::is_move_constructible_v<no_copy>);
static_assert(!std::is_move_assignable_v<no_copy>);
static_assert(!std::is_default_constructible_v<no_copy>);

struct move_no_copy {
    move_no_copy() = default;
    move_no_copy(move_no_copy&&) = default;
    move_no_copy& operator=(move_no_copy&&) = default;
    AC_DISABLE_COPY(move_no_copy)
};

static_assert(!std::is_copy_constructible_v<move_no_copy>);
static_assert(!std::is_copy_assignable_v<move_no_copy>);
/* move and default constructors are not disabled if defined explicitly */
static_assert(std::is_default_constructible_v<move_no_copy>);
static_assert(std::movable<move_no_copy>);
