/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/member_map.hpp>
#include <actl/test.hpp>

struct A {
    int a;
};

TEST("static") {
    A x{0};
    static_member_map<&A::a> map;
    get(map, x) = 1;
    ASSERT_EQUAL(1, x.a);
    static_const_member_map<&A::a> cmap;
    ASSERT_EQUAL(1, get(cmap, x));
}

TEST("non-static") {
    A x{0};
    auto map = member_map{&A::a};
    get(map, x) = 2;
    ASSERT_EQUAL(2, x.a);
    auto cmap = const_member_property_map(&A::a);
    ASSERT_EQUAL(2, get(cmap, x));
}
