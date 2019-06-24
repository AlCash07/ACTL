/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/container_map.hpp>
#include <actl/std/set.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>

TEST("const_set&") {
    const std::set<int> s{1, 5};
    auto pm = container_map{s};
    auto i0 = iterator_to_id(s, s.begin());
    auto i1 = iterator_to_id(s, --s.end());
    ASSERT_EQUAL(5, get(pm, i1));
    using C = std::vector<std::pair<container_id<std::set<int>>, int>>;
    ASSERT_EQUAL(C{{i0, 1}, {i1, 5}}, C(pm.begin(), pm.end()));
}

TEST("vector&") {
    std::vector<int> v{3, 2, 1};
    auto pm = container_map{v};
    ASSERT_EQUAL(2, get(pm, 1));
    put(pm, 2, 6);
    ASSERT_EQUAL(6, get(pm, 2));
    ASSERT_EQUAL(std::vector<int>{3, 2, 6}, v);
}

TEST("vector&&") {
    auto pm = container_map{std::vector<int>{3, 2, 1}};
    ASSERT_EQUAL(2, get(pm, 1));
    put(pm, 2, 6);
    ASSERT_EQUAL(6, get(pm, 2));
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{0, 3}, {1, 2}, {2, 6}}, C(pm.begin(), pm.end()));
}
