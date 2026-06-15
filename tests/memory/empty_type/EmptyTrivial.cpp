// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/memory/empty_type/EmptyTrivial.hpp>

struct empty {};

static_assert(ac::EmptyTrivial<empty>);
static_assert(ac::EmptyTrivial<const empty>);

static_assert(!ac::EmptyTrivial<empty&>);
static_assert(!ac::EmptyTrivial<int>);
static_assert(!ac::EmptyTrivial<void>);

/* empty non-trivial types can be defined, but they are rare in practice */

struct empty_weird_constructor {
    empty_weird_constructor() {}
};
struct empty_weird_destructor {
    ~empty_weird_destructor() {}
};
struct empty_weird_move {
    empty_weird_move(empty_weird_move&&) {}
};
struct empty_weird_copy {
    empty_weird_copy(empty_weird_copy const&) {}
};

static_assert(std::is_empty_v<empty_weird_constructor>);
static_assert(std::is_empty_v<empty_weird_destructor>);
static_assert(std::is_empty_v<empty_weird_move>);
static_assert(std::is_empty_v<empty_weird_copy>);

static_assert(!ac::EmptyTrivial<empty_weird_constructor>);
static_assert(!ac::EmptyTrivial<empty_weird_destructor>);
static_assert(!ac::EmptyTrivial<empty_weird_move>);
static_assert(!ac::EmptyTrivial<empty_weird_copy>);
