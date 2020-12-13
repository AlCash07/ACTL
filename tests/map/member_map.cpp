// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/map/member_map.hpp>
#include <actl/test.hpp>

struct A {
    int a;
};

template <class Map>
void test_member_map(Map&& map) {
    A x{0};
    get(map, x) = 1;
    ASSERT_EQUAL(1, x.a);
    ASSERT_EQUAL(1, get((const Map&)map, x));
}

TEST("static") { test_member_map(static_member_map<&A::a>{}); }

TEST("non-static") { test_member_map(member_map{&A::a}); }
