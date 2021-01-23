// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/map/pair_associative_container.hpp>
#include <actl/std/map.hpp>
#include "test.hpp"

using C = std::vector<std::pair<int, int>>;

namespace {

static C kv{{1, 1}, {2, 7}, {5, 25}};

std::map<int, int> get_map() {
    return {kv.begin(), kv.end()};
}

}  // namespace

template <bool Writable, class Map>
void test_associative_map(Map&& map) {
    for (auto [key, value] : kv) {
        CHECK(value == get(map, key));
    }
    CHECK(0 == get(map, 0));
    CHECK(0 == get(map, 3));
    C expected = kv;
    if constexpr (Writable) {
        expected.emplace_back(3, 2);
        put(map, 3, 2);
        CHECK(2 == get(map, 3));
    }
    auto r = map_range(map);
    CHECK_EQUAL_SETS(expected, {r.begin(), r.end()});
}

TEST_CASE("pair_associative_container reference") {
    auto map = get_map();
    test_associative_map<true>(map);
    CHECK(4ul == map.size());
    CHECK(2 == map[3]);
}

TEST_CASE("pair_associative_container const_reference") {
    const auto map = get_map();
    test_associative_map<false>(map);
}
