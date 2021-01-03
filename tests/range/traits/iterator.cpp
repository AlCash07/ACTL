// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/traits/iterator.hpp>
#include <actl/std/deque.hpp>
#include <actl/std/vector.hpp>

static_assert(!ac::is_contiguous_range_v<int>);
static_assert(ac::is_contiguous_range_v<int[4]>);
static_assert(ac::is_contiguous_range_v<const int[4]>);
static_assert(ac::is_contiguous_range_v<std::vector<int>>);
static_assert(!ac::is_contiguous_range_v<std::deque<int>>);

using CArray = int[2];
static_assert(ac::is_sequence_range_v<CArray>);
static_assert(ac::is_random_access_range_v<CArray>);
static_assert(ac::is_contiguous_range_v<CArray>);
static_assert(!ac::is_associative_range_v<CArray>);
