// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/traits.hpp>

static_assert(!ac::is_container_v<int>);
static_assert(!ac::is_container_v<int*>);
static_assert(!ac::is_container_v<int[]>);
static_assert(ac::is_container_v<int[2]>);
