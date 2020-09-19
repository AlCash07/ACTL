/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/functions.hpp>
#include <actl/container/hash_set.hpp>
#include <actl/functional/composite/range.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/std/all.hpp>
#include <actl/test.hpp>

static std::vector<int> xs        = {5, 4, 1, 2, 4, 2};
static std::vector<int> sorted_xs = {1, 2, 2, 4, 4, 5};
static std::vector<int> unique_xs = {1, 2, 4, 5};

static std::vector<int> xs2        = {5, 4, 1, 4};
static std::vector<int> sorted_xs2 = {1, 4, 4, 5};
static std::vector<int> unique_xs2 = {1, 4, 5};

void compare_sorted(const std::vector<int>& expected, std::vector<int> actual) {
    sort(actual);
    ASSERT_EQUAL(expected, actual);
}

TEST("list") {
    std::list<int> c;
    for (auto x : xs) {
        auto res = emplace(c, x);
        ASSERT_EQUAL(x, *res.first);
        ASSERT_TRUE(res.second);
    }
    ASSERT_EQUAL(xs, std::vector<int>(c.begin(), c.end()));
    ASSERT_EQUAL(2, *find(c, 2));
    ASSERT_EQUAL(c.end(), find(c, 3));
    erase(c, 2);
    ASSERT_EQUAL(xs2, std::vector<int>(c.begin(), c.end()));
}

TEST("set") {
    std::set<int> c;
    for (size_t i = 0; i < xs.size(); ++i) {
        auto res = emplace(c, xs[i]);
        ASSERT_EQUAL(xs[i], *res.first);
        ASSERT_EQUAL(i < 4, res.second);
    }
    ASSERT_EQUAL(unique_xs, std::vector<int>(c.begin(), c.end()));
    ASSERT_EQUAL(2, *find(c, 2));
    ASSERT_EQUAL(c.end(), find(c, 3));
    erase(c, 2);
    ASSERT_EQUAL(unique_xs2, std::vector<int>(c.begin(), c.end()));
}

TEST("multiset") {
    std::multiset<int> c;
    for (auto x : xs) {
        auto resms = emplace(c, x);
        ASSERT_EQUAL(x, *resms.first);
        ASSERT_TRUE(resms.second);
    }
    ASSERT_EQUAL(sorted_xs, std::vector<int>(c.begin(), c.end()));
    ASSERT_EQUAL(2, *find(c, 2));
    ASSERT_EQUAL(c.end(), find(c, 3));
    erase(c, 2);
    ASSERT_EQUAL(sorted_xs2, std::vector<int>(c.begin(), c.end()));
}

TEST("hash_set") {
    hash_set<int> c;
    for (size_t i = 0; i < xs.size(); ++i) {
        auto res = emplace(c, xs[i]);
        ASSERT_EQUAL(xs[i], *res.first);
        ASSERT_EQUAL(i < 4, res.second);
    }
    compare_sorted(unique_xs, std::vector<int>(c.begin(), c.end()));
    ASSERT_EQUAL(2, *find(c, 2));
    ASSERT_EQUAL(c.end(), find(c, 3));
    erase(c, 2);
    compare_sorted(unique_xs2, std::vector<int>(c.begin(), c.end()));
}

TEST("hash_multiset") {
    hash_multiset<int> c;
    for (auto x : xs) {
        auto res = emplace(c, x);
        ASSERT_EQUAL(x, *res.first);
        ASSERT_TRUE(res.second);
    }
    compare_sorted(sorted_xs, std::vector<int>(c.begin(), c.end()));
    ASSERT_EQUAL(2, *find(c, 2));
    ASSERT_EQUAL(c.end(), find(c, 3));
    erase(c, 2);
    compare_sorted(sorted_xs2, std::vector<int>(c.begin(), c.end()));
}

TEST("vector") {
    std::vector<int> c;
    for (auto x : xs) {
        auto res = emplace(c, x);
        ASSERT_EQUAL(x, *res.first);
        ASSERT_TRUE(res.second);
    }
    ASSERT_EQUAL(xs, c);
    ASSERT_EQUAL(2, *find(c, 2));
    ASSERT_EQUAL(c.end(), find(c, 3));
    erase(c, 2);
    ASSERT_EQUAL(xs2, c);
}
