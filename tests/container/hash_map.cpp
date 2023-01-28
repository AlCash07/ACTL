// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/hash_map.hpp>
#include <actl/range/traits/associative_range.hpp>
#include "test.hpp"

using HashMap = hash_map<int, int>;
static_assert(ac::Container<HashMap>);
static_assert(!ac::SequenceRange<HashMap>);
static_assert(ac::AssociativeRange<HashMap>);
static_assert(!ac::SortedRange<HashMap>);
static_assert(ac::UniqueRange<HashMap>);
static_assert(!ac::SimpleAssociativeRange<HashMap>);
static_assert(ac::PairAssociativeRange<HashMap>);

using HashMultimap = hash_multimap<int, int>;
static_assert(ac::Container<HashMultimap>);
static_assert(!ac::SequenceRange<HashMultimap>);
static_assert(ac::AssociativeRange<HashMultimap>);
static_assert(!ac::SortedRange<HashMultimap>);
static_assert(!ac::UniqueRange<HashMultimap>);
static_assert(!ac::SimpleAssociativeRange<HashMultimap>);
static_assert(ac::PairAssociativeRange<HashMultimap>);
