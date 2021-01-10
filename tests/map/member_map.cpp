// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/map/member_map.hpp>
#include "test.hpp"

struct A {
    int a;
};

template <class Map>
void test_member_map(Map&& map) {
    A x{0};
    get(map, x) = 1;
    CHECK(1 == x.a);
    CHECK(1 == get((const Map&)map, x));
}

TEST_CASE("static") {
    test_member_map(static_member_map<&A::a>{});
}

TEST_CASE("non-static") {
    test_member_map(member_map{&A::a});
}
