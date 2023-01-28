// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/string.hpp>

static_assert(ac::Container<std::string>);
static_assert(ac::SequenceRange<std::string>);
static_assert(ac::RandomAccessRange<std::string>);
static_assert(ac::ContiguousRange<std::string>);
static_assert(!ac::AssociativeRange<std::string>);
