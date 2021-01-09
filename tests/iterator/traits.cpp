// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/iterator/traits.hpp>
#include <actl/std/vector.hpp>

static_assert(!ac::is_const_iterator_v<int[4]>);
static_assert(ac::is_const_iterator_v<const int*>);
static_assert(ac::is_const_iterator_v<std::vector<int>::const_iterator>);

static_assert(!ac::is_non_const_iterator_v<int[4]>);
static_assert(ac::is_non_const_iterator_v<int*>);
static_assert(ac::is_non_const_iterator_v<std::vector<int>::iterator>);
