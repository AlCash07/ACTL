// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/range/traits/all.hpp>
#include <actl/std/list.hpp>
#include <actl/test.hpp>

TEST("category") {
    using C = std::list<int>;
    ASSERT_TRUE(is_sequence_range_v<C>);
    ASSERT_FALSE(is_random_access_range_v<C>);
    ASSERT_FALSE(is_associative_range_v<C>);
}
