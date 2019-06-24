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
    static_member_map<&A::a> pm;
    get(pm, x) = 1;
    ASSERT_EQUAL(1, x.a);
    static_const_member_map<&A::a> cpm;
    ASSERT_EQUAL(1, get(cpm, x));
}

TEST("non-static") {
    A x{0};
    auto pm = member_map{&A::a};
    get(pm, x) = 2;
    ASSERT_EQUAL(2, x.a);
    auto cpm = const_member_property_map(&A::a);
    ASSERT_EQUAL(2, get(cpm, x));
}
