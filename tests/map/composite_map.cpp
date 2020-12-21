// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/map/composite_map.hpp>
#include <actl/map/generic_container.hpp>
#include <actl/map/shift_map.hpp>
#include <actl/std/vector.hpp>

TEST_CASE("first") {
    std::vector<int> v{2, 3, 4};
    auto map = composite_map{v, static_shift_map<2>{}};
    CHECK(1 == get(map, 1));
    put(map, 2, 3);
    CHECK(std::vector<int>{2, 3, 5} == v);
    using C = std::vector<std::pair<int, int>>;
    auto r = map_range(map);
    CHECK(C{{0, 0}, {1, 1}, {2, 3}} == C{r.begin(), r.end()});
}

TEST_CASE("second") {
    std::vector<int> v{2, 3, 4};
    auto map = composite_map{static_shift_map<2>{}, v};
    CHECK(3 == get(map, 3));
    put(map, 4, 5);
    CHECK(std::vector<int>{2, 3, 5} == v);
    using C = std::vector<std::pair<int, int>>;
    auto r = map_range(map);
    CHECK(C{{2, 2}, {3, 3}, {4, 5}} == C{r.begin(), r.end()});
}

TEST_CASE("invert") {
    auto map = composite_map{static_shift_map<2>{}, static_shift_map<3>{}};
    CHECK(0 == get(map, 5));
    CHECK(7 == invert(map, 2));
}
