/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/set.hpp>
#include <actl/container/std/vector.hpp>
#include <actl/property_map/container_property_map.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("const_set&") {
    const std::set<int> s{1, 5};
    auto cpm = make_container_property_map(s);
    auto i0 = iterator_to_id(s, s.begin());
    auto i1 = iterator_to_id(s, --s.end());
    ASSERT_EQUAL(5, get(cpm, i1));
    using C = std::vector<std::pair<container_id<std::set<int>>, int>>;
    ASSERT_EQUAL(C{{i0, 1}, {i1, 5}}, C(cpm.begin(), cpm.end()));
}

TEST("vector&") {
    std::vector<int> v{3, 2, 1};
    auto cpm = make_container_property_map(v);
    ASSERT_EQUAL(2, get(cpm, 1));
    put(cpm, 2, 6);
    ASSERT_EQUAL(6, get(cpm, 2));
    ASSERT_EQUAL(std::vector<int>{3, 2, 6}, v);
}

TEST("vector&&") {
    auto cpm = make_container_property_map(std::vector<int>{3, 2, 1});
    ASSERT_EQUAL(2, get(cpm, 1));
    put(cpm, 2, 6);
    ASSERT_EQUAL(6, get(cpm, 2));
    using C = std::vector<std::pair<int, int>>;
    ASSERT_EQUAL(C{{0, 3}, {1, 2}, {2, 6}}, C(cpm.begin(), cpm.end()));
}
