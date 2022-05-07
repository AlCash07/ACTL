// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/is_range.hpp>
#include <deque>
#include <vector>
#include "test.hpp"

static_assert(!ac::is_range_v<void>);
static_assert(!ac::is_contiguous_range_v<void>);

static_assert(!ac::is_range_v<int>);
static_assert(!ac::is_contiguous_range_v<int>);

static_assert(ac::is_range_v<std::deque<int>>);
static_assert(!ac::is_contiguous_range_v<std::deque<int>>);

static_assert(ac::is_range_v<std::vector<int>>);
static_assert(ac::is_contiguous_range_v<std::vector<int>>);

using CArray = int[2];
static_assert(ac::is_range_v<CArray>);
static_assert(ac::is_range_v<CArray const>);
static_assert(ac::is_contiguous_range_v<CArray>);
