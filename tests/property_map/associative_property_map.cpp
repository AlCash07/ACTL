/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/associative_property_map.hpp>
#include <actl/test.hpp>
#include <map>

using namespace ac;

using C = std::vector<std::pair<int, int>>;

static C kv{{1, 1}, {2, 7}, {5, 25}};

inline std::map<int, int> get_map() { return {kv.begin(), kv.end()}; }

template <bool Writable, class Map>
inline void test_associative_pm(Map&& map) {
    auto apm = make_associative_property_map(std::forward<Map>(map));
    for (auto[key, value] : kv) {
        ASSERT_EQUAL(value, get(apm, key));
    }
    ASSERT_EQUAL(0, get(apm, 0));
    ASSERT_EQUAL(0, get(apm, 3));
    C expected = kv;
    if constexpr (Writable) {
        expected.emplace_back(3, 2);
        put(apm, 3, 2);
        ASSERT_EQUAL(2, get(apm, 3));
    }
    ASSERT_EQUAL_SETS(expected, C(apm.begin(), apm.end()));
}

TEST("associative_property_map::reference") {
    auto map = get_map();
    test_associative_pm<true>(map);
    ASSERT_EQUAL(4, map.size());
    ASSERT_EQUAL(2, map[3]);
}

TEST("associative_property_map::const_reference") {
    const auto map = get_map();
    test_associative_pm<false>(map);
}

TEST("associative_property_map::rvalue_reference") { test_associative_pm<true>(get_map()); }
