/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/associative_map.hpp>
#include <actl/std/map.hpp>
#include <actl/test.hpp>

using C = std::vector<std::pair<int, int>>;

static C kv{{1, 1}, {2, 7}, {5, 25}};

inline std::map<int, int> get_map() { return {kv.begin(), kv.end()}; }

template <bool Writable, class Map>
inline void test_associative_map(Map&& map) {
    auto amap = associative_map{std::forward<Map>(map)};
    for (auto [key, value] : kv) {
        ASSERT_EQUAL(value, get(amap, key));
    }
    ASSERT_EQUAL(0, get(amap, 0));
    ASSERT_EQUAL(0, get(amap, 3));
    C expected = kv;
    if constexpr (Writable) {
        expected.emplace_back(3, 2);
        put(amap, 3, 2);
        ASSERT_EQUAL(2, get(amap, 3));
    }
    ASSERT_EQUAL_SETS(expected, {amap.begin(), amap.end()});
}

TEST("reference") {
    auto map = get_map();
    test_associative_map<true>(map);
    ASSERT_EQUAL(4u, map.size());
    ASSERT_EQUAL(2, map[3]);
}

TEST("const_reference") {
    const auto map = get_map();
    test_associative_map<false>(map);
}

TEST("rvalue_reference") { test_associative_map<true>(get_map()); }
