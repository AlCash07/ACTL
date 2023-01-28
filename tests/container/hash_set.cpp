// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/hash_set.hpp>
#include <actl/range/traits/associative_range.hpp>
#include "test.hpp"

using HashSet = hash_set<int>;
static_assert(ac::Container<HashSet>);
static_assert(!ac::SequenceRange<HashSet>);
static_assert(ac::AssociativeRange<HashSet>);
static_assert(!ac::SortedRange<HashSet>);
static_assert(ac::UniqueRange<HashSet>);
static_assert(ac::SimpleAssociativeRange<HashSet>);
static_assert(!ac::PairAssociativeRange<HashSet>);

using HashMultiset = hash_multiset<int>;
static_assert(ac::Container<HashMultiset>);
static_assert(!ac::SequenceRange<HashMultiset>);
static_assert(ac::AssociativeRange<HashMultiset>);
static_assert(!ac::SortedRange<HashMultiset>);
static_assert(!ac::UniqueRange<HashMultiset>);
static_assert(ac::SimpleAssociativeRange<HashMultiset>);
static_assert(!ac::PairAssociativeRange<HashMultiset>);
