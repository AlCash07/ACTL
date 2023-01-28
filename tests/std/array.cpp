// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/array.hpp>

using Array = std::array<int, 2>;
static_assert(ac::Container<Array>);
static_assert(ac::SequenceRange<Array>);
static_assert(ac::RandomAccessRange<Array>);
static_assert(ac::ContiguousRange<Array>);
static_assert(!ac::AssociativeRange<Array>);
