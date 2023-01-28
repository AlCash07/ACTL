// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/set.hpp>

using Set = std::set<int>;
static_assert(ac::Container<Set>);
static_assert(!ac::SequenceRange<Set>);
static_assert(ac::AssociativeRange<Set>);
static_assert(ac::SortedRange<Set>);
static_assert(ac::UniqueRange<Set>);
static_assert(ac::SimpleAssociativeRange<Set>);
static_assert(!ac::PairAssociativeRange<Set>);

using Multiset = std::multiset<int>;
static_assert(ac::Container<Multiset>);
static_assert(!ac::SequenceRange<Multiset>);
static_assert(ac::AssociativeRange<Multiset>);
static_assert(ac::SortedRange<Multiset>);
static_assert(!ac::UniqueRange<Multiset>);
static_assert(ac::SimpleAssociativeRange<Multiset>);
static_assert(!ac::PairAssociativeRange<Multiset>);
