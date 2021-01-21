// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/category/range.hpp>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include "test.hpp"

static_assert(ac::is_forward_range_v<std::forward_list<int>>);
static_assert(!ac::is_random_access_range_v<std::forward_list<int>>);

static_assert(ac::is_forward_range_v<std::list<int>>);
static_assert(ac::is_bidirectional_range_v<std::list<int>>);
static_assert(!ac::is_random_access_range_v<std::list<int>>);

static_assert(ac::is_forward_range_v<std::deque<int>>);
static_assert(ac::is_bidirectional_range_v<std::deque<int>>);
static_assert(ac::is_random_access_range_v<std::deque<int>>);

static_assert(ac::is_forward_range_v<std::vector<int>>);
static_assert(ac::is_bidirectional_range_v<std::vector<int>>);
static_assert(ac::is_random_access_range_v<std::vector<int>>);

static_assert(ac::is_random_access_range_v<int[2]>);
