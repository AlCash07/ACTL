// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include "map/logging_map.hpp"
#include <actl/map/generic_container.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

TEST_CASE("logging_map vector") {
    std::vector<int> v(4);
    using Log = std::vector<std::pair<int, int>>;
    Log log;
    auto map = logging_map{v, std::back_inserter(log)};
    put(map, 1, 2);
    put(map, 2, 4);
    CHECK(2 == get(map, 1));
    put(map, 1, 3);
    CHECK(Log{{1, 2}, {2, 4}, {1, 3}} == log);
}
