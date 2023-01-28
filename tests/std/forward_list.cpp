// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/forward_list.hpp>

using ForwardList = std::forward_list<int>;
static_assert(ac::Container<ForwardList>);
static_assert(ac::SequenceRange<ForwardList>);
static_assert(!ac::RandomAccessRange<ForwardList>);
static_assert(!ac::AssociativeRange<ForwardList>);
