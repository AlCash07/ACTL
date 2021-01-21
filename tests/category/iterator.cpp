// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/category/iterator.hpp>
#include <forward_list>
#include <list>
#include <vector>
#include "test.hpp"

using InputIter = std::istream_iterator<int>;
static_assert(ac::is_input_iterator_v<InputIter>);
static_assert(!ac::is_forward_iterator_v<InputIter>);

using OutputIter = std::ostream_iterator<int>;
static_assert(!ac::is_input_iterator_v<OutputIter>);
static_assert(!ac::is_forward_iterator_v<OutputIter>);

using ForwardIter = std::forward_list<int>::iterator;
static_assert(ac::is_forward_iterator_v<ForwardIter>);
static_assert(!ac::is_random_access_iterator_v<ForwardIter>);

using BidirIter = std::list<int>::iterator;
static_assert(ac::is_forward_iterator_v<BidirIter>);
static_assert(ac::is_bidirectional_iterator_v<BidirIter>);
static_assert(!ac::is_random_access_iterator_v<BidirIter>);

using RandAccessIter = std::vector<int>::iterator;
static_assert(ac::is_forward_iterator_v<RandAccessIter>);
static_assert(ac::is_bidirectional_iterator_v<RandAccessIter>);
static_assert(ac::is_random_access_iterator_v<RandAccessIter>);
