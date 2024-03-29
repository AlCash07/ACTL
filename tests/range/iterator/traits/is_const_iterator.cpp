// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/iterator/traits/is_const_iterator.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

static_assert(!ac::is_const_iterator_v<int[4]>);
static_assert(ac::is_const_iterator_v<int const*>);
static_assert(ac::is_const_iterator_v<std::vector<int>::const_iterator>);

static_assert(!ac::is_non_const_iterator_v<int[4]>);
static_assert(ac::is_non_const_iterator_v<int*>);
static_assert(ac::is_non_const_iterator_v<std::vector<int>::iterator>);
