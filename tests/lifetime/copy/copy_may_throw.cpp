// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/concepts/object/Copyable.hpp>
#include <actl_test/core/copy_may_throw.hpp>

struct tests_copy : ac::copy_may_throw {};

static_assert(std::copyable<tests_copy>);
static_assert(!ac::TriviallyCopyable<tests_copy>);
static_assert(!ac::NothrowCopyable<tests_copy>);
/* move is not affected */
static_assert(ac::TriviallyMovable<tests_copy>);
static_assert(ac::NothrowMovable<tests_copy>);
/* default constructor is not affected */
static_assert(std::is_default_constructible_v<tests_copy>);
