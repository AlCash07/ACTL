/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/vector.hpp>
#include <actl/property_map/transition_property_map.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("to_bool") {
    auto tpm = make_transition_property_map(std::vector<int>{3, 2, 0, 6});
    ASSERT_EQUAL(3, get(tpm, 0));
    put(tpm, 1, 0);
    ASSERT_EQUAL_SETS(std::vector<std::pair<int, int>>{{0, 3}, {3, 6}}, {tpm.begin(), tpm.end()});
}

TEST("lambda") {
    int  x0;
    auto tpm =
        make_transition_property_map(std::vector<int>{3, 2, 6}, [&x0](int x) { return x != x0; });
    x0 = 3;
    ASSERT_EQUAL_SETS(std::vector<std::pair<int, int>>{{1, 2}, {2, 6}}, {tpm.begin(), tpm.end()});
}
