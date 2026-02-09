// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/lifetime/move/AC_DISABLE_MOVE.hpp>
#include <type_traits>

struct no_move {
    AC_DISABLE_MOVE(no_move)
};

static_assert(!std::is_move_constructible_v<no_move>);
static_assert(!std::is_move_assignable_v<no_move>);
/* copy and default constructor are also disabled */
static_assert(!std::is_copy_constructible_v<no_move>);
static_assert(!std::is_copy_assignable_v<no_move>);
static_assert(!std::is_default_constructible_v<no_move>);
