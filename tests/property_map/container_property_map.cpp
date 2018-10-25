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
    const std::set<int> s{1, 4, 5};
    auto cpm = make_container_property_map(s);
    ASSERT_EQUAL(1, get(cpm, iterator_to_id(s, s.begin())));
    ASSERT_EQUAL(5, get(cpm, iterator_to_id(s, --s.end())));
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
}
