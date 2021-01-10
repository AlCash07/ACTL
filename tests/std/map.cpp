// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/std/map.hpp>
#include "test.hpp"

using Map = std::map<int, int>;
static_assert(ac::is_container_v<Map>);
static_assert(!ac::is_sequence_range_v<Map>);
static_assert(ac::is_associative_range_v<Map>);
static_assert(!ac::is_simple_associative_range_v<Map>);
static_assert(ac::is_pair_associative_range_v<Map>);
static_assert(ac::is_sorted_range_v<Map>);
static_assert(ac::is_unique_range_v<Map>);

using Multimap = std::multimap<int, int>;
static_assert(ac::is_container_v<Multimap>);
static_assert(!ac::is_sequence_range_v<Multimap>);
static_assert(ac::is_associative_range_v<Multimap>);
static_assert(!ac::is_simple_associative_range_v<Multimap>);
static_assert(ac::is_pair_associative_range_v<Multimap>);
static_assert(ac::is_sorted_range_v<Multimap>);
static_assert(!ac::is_unique_range_v<Multimap>);
