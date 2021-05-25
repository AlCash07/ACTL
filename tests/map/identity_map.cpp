// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/map/identity_map.hpp>
#include "test.hpp"

TEST_CASE("identity_map int")
{
    identity_map<int, long long> map;
    CHECK(1LL == get(map, 1));
    CHECK(3 == invert(map, 3LL));
}

struct A
{
    int a;
};

struct B : A
{
    int b;
};

TEST_CASE("identity_map reference")
{
    B x{{0}, 0};
    identity_map<B&, A&> map;
    get(map, x).a = 2;
    CHECK(2 == x.a);
}
