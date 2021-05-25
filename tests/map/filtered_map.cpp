// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/map/filtered_map.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

TEST_CASE("filtered_map to_bool")
{
    auto map = filtered_map{std::vector<int>{3, 2, 0, 6}};
    CHECK(3 == get(map, 0));
    put(map, 1, 0);
    using C = std::vector<std::pair<int, int>>;
    auto r = map_range(map);
    CHECK(C{{0, 3}, {3, 6}} == C{r.begin(), r.end()});
}

TEST_CASE("filtered_map lambda")
{
    int x0;
    auto map = filtered_map{
        std::vector<int>{3, 2, 6},
        [&x0](int x)
        {
            return x != x0;
        }};
    x0 = 3;
    using C = std::vector<std::pair<int, int>>;
    auto r = map_range(map);
    CHECK_EQUAL_SETS(C{{1, 2}, {2, 6}}, C{r.begin(), r.end()});
}
