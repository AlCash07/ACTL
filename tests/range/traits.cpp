// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/range/traits.hpp>
#include "test.hpp"

static_assert(!ac::is_container_v<int>);
static_assert(!ac::is_container_v<int*>);
static_assert(!ac::is_container_v<int[]>);

using CArray = int[2];
static_assert(ac::is_container_v<CArray>);
static_assert(ac::is_sequence_range_v<CArray>);
static_assert(!ac::is_associative_range_v<CArray>);
