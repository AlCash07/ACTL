// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/category.hpp>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>

static_assert(!ac::is_range_v<void>);
static_assert(!ac::is_contiguous_range_v<void>);

static_assert(!ac::is_range_v<int>);
static_assert(!ac::is_contiguous_range_v<int>);

static_assert(ac::is_forward_range_v<std::forward_list<int>>);
static_assert(!ac::is_random_access_range_v<std::forward_list<int>>);

static_assert(ac::is_forward_range_v<std::list<int>>);
static_assert(ac::is_bidirectional_range_v<std::list<int>>);
static_assert(!ac::is_random_access_range_v<std::list<int>>);

static_assert(ac::is_range_v<std::deque<int>>);
static_assert(ac::is_forward_range_v<std::deque<int>>);
static_assert(ac::is_bidirectional_range_v<std::deque<int>>);
static_assert(ac::is_random_access_range_v<std::deque<int>>);
static_assert(!ac::is_contiguous_range_v<std::deque<int>>);

static_assert(ac::is_range_v<std::vector<int>>);
static_assert(ac::is_forward_range_v<std::vector<int>>);
static_assert(ac::is_bidirectional_range_v<std::vector<int>>);
static_assert(ac::is_random_access_range_v<std::vector<int>>);
static_assert(ac::is_contiguous_range_v<std::vector<int>>);

using CArray = int[2];
static_assert(ac::is_range_v<CArray>);
static_assert(ac::is_range_v<CArray const>);
static_assert(ac::is_random_access_range_v<CArray>);
static_assert(ac::is_contiguous_range_v<CArray>);
