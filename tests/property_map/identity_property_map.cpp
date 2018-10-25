/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/identity_property_map.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("int") {
    identity_property_map<int, long long> ipm;
    ASSERT_EQUAL(1LL, get(ipm, 1));
    ASSERT_EQUAL(3, ipm.invert(3LL));
}

struct A {
    int a;
};

struct B : A {
    int b;
};

TEST("reference") {
    B x{{0}, 0};
    identity_property_map<B&, A&> ipm;
    get(ipm, x).a = 2;
    ASSERT_EQUAL(2, x.a);
}
