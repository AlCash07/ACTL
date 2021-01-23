// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/std/set.hpp>
#include "test.hpp"

using Set = std::set<int>;
static_assert(ac::is_container_v<Set>);
static_assert(!ac::is_sequence_range_v<Set>);
static_assert(ac::is_associative_range_v<Set>);
static_assert(ac::is_sorted_range_v<Set>);
static_assert(ac::is_unique_range_v<Set>);
static_assert(ac::is_simple_associative_range_v<Set>);
static_assert(!ac::is_pair_associative_range_v<Set>);

using Multiset = std::multiset<int>;
static_assert(ac::is_container_v<Multiset>);
static_assert(!ac::is_sequence_range_v<Multiset>);
static_assert(ac::is_associative_range_v<Multiset>);
static_assert(ac::is_sorted_range_v<Multiset>);
static_assert(!ac::is_unique_range_v<Multiset>);
static_assert(ac::is_simple_associative_range_v<Multiset>);
static_assert(!ac::is_pair_associative_range_v<Multiset>);
