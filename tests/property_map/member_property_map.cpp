/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/member_property_map.hpp>
#include <actl/test.hpp>

using namespace ac;

struct A {
    int a;
};

TEST("static") {
    A x{0};
    static_member_property_map<&A::a> mpm;
    get(mpm, x) = 1;
    ASSERT_EQUAL(1, x.a);
    static_const_member_property_map<&A::a> cmpm;
    ASSERT_EQUAL(1, get(cmpm, x));
}

TEST("non-static") {
    A x{0};
    auto mpm = make_member_property_map(&A::a);
    get(mpm, x) = 2;
    ASSERT_EQUAL(2, x.a);
    auto cmpm = make_const_member_property_map(&A::a);
    ASSERT_EQUAL(2, get(cmpm, x));
}
