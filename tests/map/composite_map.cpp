/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/composite_map.hpp>
#include <actl/map/container_map.hpp>
#include <actl/map/shift_map.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>

TEST("first") {
    std::vector<int> v{2, 3, 4};
    auto map = composite_map{container_map{v}, static_shift_map<2>{}};
    ASSERT_EQUAL(1, get(map, 1));
    put(map, 2, 3);
    ASSERT_EQUAL(std::vector<int>{2, 3, 5}, v);
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{0, 0}, {1, 1}, {2, 3}}, C(map.begin(), map.end()));
}

TEST("second") {
    std::vector<int> v{2, 3, 4};
    auto map = composite_map{static_shift_map<2>{}, container_map{v}};
    ASSERT_EQUAL(3, get(map, 3));
    put(map, 4, 5);
    ASSERT_EQUAL(std::vector<int>{2, 3, 5}, v);
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{2, 2}, {3, 3}, {4, 5}}, C(map.begin(), map.end()));
}

TEST("invert") {
    auto map = composite_map{static_shift_map<2>{}, static_shift_map<3>{}};
    ASSERT_EQUAL(0, get(map, 5));
    ASSERT_EQUAL(7, map.invert(2));
}