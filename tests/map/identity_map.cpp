/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/identity_map.hpp>
#include <actl/test.hpp>

TEST("int") {
    identity_map<int, long long> map;
    ASSERT_EQUAL(1LL, get(map, 1));
    ASSERT_EQUAL(3, invert(map, 3LL));
}

struct A {
    int a;
};

struct B : A {
    int b;
};

TEST("reference") {
    B x{{0}, 0};
    identity_map<B&, A&> map;
    get(map, x).a = 2;
    ASSERT_EQUAL(2, x.a);
}
