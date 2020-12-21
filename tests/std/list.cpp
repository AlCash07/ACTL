// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/traits/all.hpp>
#include <actl/std/list.hpp>

TEST_CASE("list category") {
    using C = std::list<int>;
    CHECK(is_sequence_range_v<C>);
    CHECK_FALSE(is_random_access_range_v<C>);
    CHECK_FALSE(is_associative_range_v<C>);
}
