// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/properties.hpp>
#include <actl/std/vector.hpp>

using Vector = std::vector<int>;
static_assert(ac::SequenceRange<Vector>);
static_assert(ac::RandomAccessRange<Vector>);
static_assert(ac::ContiguousRange<Vector>);
static_assert(!ac::AssociativeRange<Vector>);
