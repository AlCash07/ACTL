// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>

static_assert(!ac::Container<int>);
static_assert(!ac::Container<int*>);
static_assert(!ac::Container<int[]>);

using CArray = int[2];
static_assert(ac::Container<CArray>);
static_assert(ac::SequenceRange<CArray>);
static_assert(!ac::AssociativeRange<CArray>);
