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
    auto pm = make_transition_property_map(std::vector<int>{3, 2, 0, 6});
    ASSERT_EQUAL(3, get(pm, 0));
    put(pm, 1, 0);
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{0, 3}, {3, 6}}, C(pm.begin(), pm.end()));
}

TEST("lambda") {
    int  x0;
    auto pm =
        make_transition_property_map(std::vector<int>{3, 2, 6}, [&x0](int x) { return x != x0; });
    x0 = 3;
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL_SETS(C{{1, 2}, {2, 6}}, C(pm.begin(), pm.end()));
}
