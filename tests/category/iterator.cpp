// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/category/iterator.hpp>
#include <actl/std/forward_list.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>

static_assert(!ac::is_iterator_v<void>);
static_assert(!ac::is_iterator_v<int[4]>);
static_assert(!ac::is_iterator_v<void*>);
static_assert(!ac::is_iterator_v<int (*)()>);
static_assert(ac::is_iterator_v<int*>);
static_assert(ac::is_iterator_v<std::vector<int>::iterator>);

using InputIter = std::istream_iterator<int>;
static_assert(!ac::is_output_iterator_v<InputIter>);
static_assert(ac::is_input_iterator_v<InputIter>);
static_assert(!ac::is_forward_iterator_v<InputIter>);

using OutputIter = std::ostream_iterator<int>;
static_assert(!ac::is_input_iterator_v<OutputIter>);
static_assert(ac::is_output_iterator_v<OutputIter>);
static_assert(!ac::is_forward_iterator_v<OutputIter>);

using ForwardIter = std::forward_list<int>::iterator;
static_assert(!ac::is_output_iterator_v<ForwardIter>);
static_assert(ac::is_forward_iterator_v<ForwardIter>);
static_assert(!ac::is_random_access_iterator_v<ForwardIter>);

using BidirIter = std::list<int>::iterator;
static_assert(!ac::is_output_iterator_v<BidirIter>);
static_assert(ac::is_forward_iterator_v<BidirIter>);
static_assert(ac::is_bidirectional_iterator_v<BidirIter>);
static_assert(!ac::is_random_access_iterator_v<BidirIter>);

using RandAccessIter = std::vector<int>::iterator;
static_assert(!ac::is_output_iterator_v<RandAccessIter>);
static_assert(ac::is_forward_iterator_v<RandAccessIter>);
static_assert(ac::is_bidirectional_iterator_v<RandAccessIter>);
static_assert(ac::is_random_access_iterator_v<RandAccessIter>);
