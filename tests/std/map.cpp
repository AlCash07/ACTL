// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/map.hpp>

using Map = std::map<int, int>;
static_assert(ac::Container<Map>);
static_assert(!ac::SequenceRange<Map>);
static_assert(ac::AssociativeRange<Map>);
static_assert(!ac::SimpleAssociativeRange<Map>);
static_assert(ac::PairAssociativeRange<Map>);
static_assert(ac::SortedRange<Map>);
static_assert(ac::UniqueRange<Map>);

using Multimap = std::multimap<int, int>;
static_assert(ac::Container<Multimap>);
static_assert(!ac::SequenceRange<Multimap>);
static_assert(ac::AssociativeRange<Multimap>);
static_assert(!ac::SimpleAssociativeRange<Multimap>);
static_assert(ac::PairAssociativeRange<Multimap>);
static_assert(ac::SortedRange<Multimap>);
static_assert(!ac::UniqueRange<Multimap>);
