// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/properties.hpp>
#include <actl/std/forward_list.hpp>

using ForwardList = std::forward_list<int>;
static_assert(ac::is_container_v<ForwardList>);
static_assert(ac::is_sequence_range_v<ForwardList>);
static_assert(!ac::is_random_access_range_v<ForwardList>);
static_assert(!ac::is_associative_range_v<ForwardList>);
