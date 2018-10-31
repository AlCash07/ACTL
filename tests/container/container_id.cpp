/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/container_id.hpp>
#include <actl/container/std/all.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("vector") {
    using vid = container_id<std::vector<int>>;
    ASSERT_TRUE(std::is_same_v<int, vid>);
    std::vector<int> c = {0, 2, 4};
    for (auto id : id_range(c)) {
        ASSERT_EQUAL(2 * id, c[(unsigned)id]);
    }
    auto id = begin_id(c);
    id_erase(c, ++id);
    ASSERT_EQUAL(std::vector<int>{0, 4}, c);
}

TEST("set") {
    std::set<int> c = {0, 2, 4};
    int i = 0;
    for (auto id : id_range(c)) {
        ASSERT_EQUAL(2 * i, id_at(c, id));
        ++i;
    }
    id_erase(c, ++begin_id(c));
    ASSERT_EQUAL(std::set<int>{0, 4}, c);
}

TEST("id_key") {
    using sid = container_id<std::set<int>>;
    std::set<int> c = {0, 2, 4};
    std::set<sid> s;
    s.insert(begin_id(c));
    ASSERT_EQUAL(1u, s.size());
    s.insert(end_id(c));
    ASSERT_EQUAL(2u, s.size());
    s.insert(begin_id(c));
    ASSERT_EQUAL(2u, s.size());
    std::unordered_set<sid> us;
    us.insert(begin_id(c));
    ASSERT_EQUAL(1u, us.size());
    us.insert(end_id(c));
    ASSERT_EQUAL(2u, us.size());
    us.insert(begin_id(c));
    ASSERT_EQUAL(2u, us.size());
}
