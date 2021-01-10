// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/map/generic_container.hpp>
#include <actl/std/set.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

TEST_CASE("const_set&") {
    const std::set<int> s{1, 5};
    auto i0 = iterator_to_id(s, s.begin());
    auto i1 = iterator_to_id(s, --s.end());
    CHECK(5 == get(s, i1));
    using C = std::vector<std::pair<container_id<std::set<int>>, int>>;
    auto r = map_range(s);
    CHECK(C{{i0, 1}, {i1, 5}} == C{r.begin(), r.end()});
}

TEST_CASE("vector&") {
    std::vector<int> v{3, 2, 1};
    CHECK(2 == get(v, 1));
    put(v, 2, 6);
    CHECK(6 == get(v, 2));
    CHECK(std::vector<int>{3, 2, 6} == v);
    using C = std::vector<std::pair<int, int>>;
    auto r = map_range(v);
    CHECK(C{{0, 3}, {1, 2}, {2, 6}} == C{r.begin(), r.end()});
}
