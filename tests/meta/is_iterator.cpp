// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/is_iterator.hpp>
#include <vector>
#include "test.hpp"

static_assert(!ac::is_iterator_v<void>);
static_assert(!ac::is_iterator_v<int[4]>);
static_assert(!ac::is_iterator_v<void*>);
static_assert(!ac::is_iterator_v<int (*)()>);
static_assert(ac::is_iterator_v<int*>);
static_assert(ac::is_iterator_v<std::vector<int>::iterator>);

// static_assert(!ac::is_output_iterator_v<std::istream_iterator<int>>);
// static_assert(ac::is_output_iterator_v<std::ostream_iterator<int>>);
// static_assert(ac::is_output_iterator_v<std::vector<int>::iterator>);
// static_assert(!ac::is_output_iterator_v<std::vector<int>::const_iterator>);
