// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/container/functions.hpp>
#include <actl/container/hash_set.hpp>
#include <actl/functional/composite/range.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/std/all.hpp>

static std::vector<int> xs        = {5, 4, 1, 2, 4, 2};
static std::vector<int> sorted_xs = {1, 2, 2, 4, 4, 5};
static std::vector<int> unique_xs = {1, 2, 4, 5};

static std::vector<int> xs2        = {5, 4, 1, 4};
static std::vector<int> sorted_xs2 = {1, 4, 4, 5};
static std::vector<int> unique_xs2 = {1, 4, 5};

void compare_sorted(const std::vector<int>& expected, std::vector<int> actual) {
    sort(actual);
    CHECK(expected == actual);
}

TEST_CASE("list functions") {
    std::list<int> c;
    for (auto x : xs) {
        auto res = emplace(c, x);
        CHECK(x == *res.first);
        CHECK(res.second);
    }
    CHECK(xs == std::vector<int>(c.begin(), c.end()));
    CHECK(2 == *find(c, 2));
    CHECK(c.end() == find(c, 3));
    erase(c, 2);
    CHECK(xs2 == std::vector<int>(c.begin(), c.end()));
}

TEST_CASE("set functions") {
    std::set<int> c;
    for (size_t i = 0; i < xs.size(); ++i) {
        auto res = emplace(c, xs[i]);
        CHECK(xs[i] == *res.first);
        CHECK((i < 4) == res.second);
    }
    CHECK(unique_xs == std::vector<int>(c.begin(), c.end()));
    CHECK(2 == *find(c, 2));
    CHECK(c.end() == find(c, 3));
    erase(c, 2);
    CHECK(unique_xs2 == std::vector<int>(c.begin(), c.end()));
}

TEST_CASE("multiset functions") {
    std::multiset<int> c;
    for (auto x : xs) {
        auto resms = emplace(c, x);
        CHECK(x == *resms.first);
        CHECK(resms.second);
    }
    CHECK(sorted_xs == std::vector<int>(c.begin(), c.end()));
    CHECK(2 == *find(c, 2));
    CHECK(c.end() == find(c, 3));
    erase(c, 2);
    CHECK(sorted_xs2 == std::vector<int>(c.begin(), c.end()));
}

TEST_CASE("hash_set functions") {
    hash_set<int> c;
    for (size_t i = 0; i < xs.size(); ++i) {
        auto res = emplace(c, xs[i]);
        CHECK(xs[i] == *res.first);
        bool inserted = i < 4;
        CHECK(inserted == res.second);
    }
    compare_sorted(unique_xs, std::vector<int>(c.begin(), c.end()));
    CHECK(2 == *find(c, 2));
    CHECK(c.end() == find(c, 3));
    erase(c, 2);
    compare_sorted(unique_xs2, std::vector<int>(c.begin(), c.end()));
}

TEST_CASE("hash_multiset functions") {
    hash_multiset<int> c;
    for (auto x : xs) {
        auto res = emplace(c, x);
        CHECK(x == *res.first);
        CHECK(res.second);
    }
    compare_sorted(sorted_xs, std::vector<int>(c.begin(), c.end()));
    CHECK(2 == *find(c, 2));
    CHECK(c.end() == find(c, 3));
    erase(c, 2);
    compare_sorted(sorted_xs2, std::vector<int>(c.begin(), c.end()));
}

TEST_CASE("vector functions") {
    std::vector<int> c;
    for (auto x : xs) {
        auto res = emplace(c, x);
        CHECK(x == *res.first);
        CHECK(res.second);
    }
    CHECK(xs == c);
    CHECK(2 == *find(c, 2));
    CHECK(c.end() == find(c, 3));
    erase(c, 2);
    CHECK(xs2 == c);
}
