// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/list.hpp>

using List = std::list<int>;
static_assert(ac::Container<List>);
static_assert(ac::SequenceRange<List>);
static_assert(!ac::RandomAccessRange<List>);
static_assert(!ac::AssociativeRange<List>);
