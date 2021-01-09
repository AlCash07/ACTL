// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/container/container_id.hpp>
#include <actl/container/hash_set.hpp>
#include <actl/functional/range/comparison.hpp>
#include <actl/std/all.hpp>

TEST_CASE("vector id") {
    using vid = container_id<std::vector<int>>;
    CHECK(std::is_same_v<int, vid>);
    std::vector<int> c = {0, 2, 4};
    for (auto id : id_range(c)) {
        CHECK(2 * id == c[(unsigned)id]);
    }
    auto id = id_begin(c);
    id_erase(c, ++id);
    CHECK(std::vector<int>{0, 4} == c);
}

TEST_CASE("set id") {
    std::set<int> c = {0, 2, 4};
    int i = 0;
    for (auto id : id_range(c)) {
        CHECK(2 * i == id_at(c, id));
        ++i;
    }
    id_erase(c, ++id_begin(c));
    CHECK(std::set<int>{0, 4} == c);
}

TEST_CASE("id_key") {
    using sid = container_id<std::set<int>>;
    std::set<int> c = {0, 2, 4};
    std::set<sid> s;
    s.insert(id_begin(c));
    CHECK(1ul == s.size());
    s.insert(id_end(c));
    CHECK(2ul == s.size());
    s.insert(id_begin(c));
    CHECK(2ul == s.size());
    hash_set<sid> us;
    us.insert(id_begin(c));
    CHECK(1ul == us.size());
    us.insert(id_end(c));
    CHECK(2ul == us.size());
    us.insert(id_begin(c));
    CHECK(2ul == us.size());
}
