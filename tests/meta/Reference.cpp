// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/Reference.hpp>

static_assert(!ac::Reference<void>);
static_assert(!ac::Reference<int>);
static_assert(!ac::Reference<int*>);
static_assert(ac::Reference<int&>);
static_assert(ac::Reference<int&&>);
static_assert(ac::Reference<int const&>);
