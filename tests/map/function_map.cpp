// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/map/function_map.hpp>
#include <cstring>

TEST_CASE("function_map free_function") {
    auto map = function_map{std::strlen};
    CHECK(0ul == get(map, ""));
    CHECK(3ul == get(map, "map"));
}

TEST_CASE("function_map lambda") {
    int count = 0;
    auto map = function_map{[&count](int) { return count++; }};
    CHECK(0 == get(map, 0));
    CHECK(1 == get(map, 0));
    CHECK(2 == count);
}
