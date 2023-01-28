// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/deque.hpp>

using Deque = std::deque<int>;
static_assert(ac::Container<Deque>);
static_assert(ac::SequenceRange<Deque>);
static_assert(ac::RandomAccessRange<Deque>);
static_assert(!ac::ContiguousRange<Deque>);
static_assert(!ac::AssociativeRange<Deque>);
