/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/util/compressed_pair.hpp>
#include <actl/util/none.hpp>

TEST("member_access") {
    compressed_pair<int, std::string> p{2, 'a', 'b', 'a'};
    ASSERT_EQUAL(2, p.first());
    ASSERT_EQUAL("aba", p.second());
    p.first() = 4;
    p.second() = "s";
    ASSERT_EQUAL(4, p.first());
    ASSERT_EQUAL("s", p.second());
    compressed_pair<none, int> p2{none{}, 3};
    ASSERT_EQUAL(3, p2.second());
}

using pii = compressed_pair<int, int>;

TEST("equal") {
    ASSERT_TRUE(pii{0, 1} == pii{0, 1});
    ASSERT_FALSE(pii{0, 1} == pii{0, 0});
    ASSERT_FALSE(pii{0, 1} == pii{1, 1});
    ASSERT_FALSE(pii{0, 1} == pii{1, 0});
}

TEST("less") {
    ASSERT_FALSE(pii{0, 0} < pii{0, 0});
    ASSERT_TRUE(pii{0, 0} < pii{0, 1});
    ASSERT_TRUE(pii{0, 1} < pii{1, 0});
    ASSERT_FALSE(pii{1, 0} < pii{0, 1});
}

struct fin final : none {};

TEST("sizeof") {
    ASSERT_EQUAL(2 * sizeof(int), sizeof(compressed_pair<int, int>));
    // TODO: investigate why the following size isn't 1.
    ASSERT_TRUE(2 >= sizeof(compressed_pair<none, none>));
    ASSERT_EQUAL(sizeof(int), sizeof(compressed_pair<none, int>));
    ASSERT_EQUAL(sizeof(int), sizeof(compressed_pair<int, none>));
    ASSERT_TRUE(sizeof(int) < sizeof(compressed_pair<fin, int>));
    ASSERT_TRUE(sizeof(int) < sizeof(compressed_pair<int, fin>));
}
