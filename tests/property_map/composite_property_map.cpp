/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/vector.hpp>
#include <actl/property_map/composite_property_map.hpp>
#include <actl/property_map/container_property_map.hpp>
#include <actl/property_map/shift_property_map.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("first") {
    std::vector<int> v{2, 3, 4};
    auto pm = composite_property_map(container_property_map(v), static_shift_property_map<2>{});
    ASSERT_EQUAL(1, get(pm, 1));
    put(pm, 2, 3);
    ASSERT_EQUAL(std::vector<int>{2, 3, 5}, v);
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{0, 0}, {1, 1}, {2, 3}}, C(pm.begin(), pm.end()));
}

TEST("second") {
    std::vector<int> v{2, 3, 4};
    auto pm = composite_property_map(static_shift_property_map<2>{}, container_property_map(v));
    ASSERT_EQUAL(3, get(pm, 3));
    put(pm, 4, 5);
    ASSERT_EQUAL(std::vector<int>{2, 3, 5}, v);
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{2, 2}, {3, 3}, {4, 5}}, C(pm.begin(), pm.end()));
}

TEST("invert") {
    auto pm =
        composite_property_map(static_shift_property_map<2>{}, static_shift_property_map<3>{});
    ASSERT_EQUAL(0, get(pm, 5));
    ASSERT_EQUAL(7, pm.invert(2));
}
