// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/utility/compressed_pair.hpp>
#include <actl/utility/none.hpp>
#include "test.hpp"

TEST_CASE("member_access") {
    compressed_pair<int, std::string> p{2, 'a', 'b', 'a'};
    CHECK(2 == p.first());
    CHECK("aba"sv == p.second());
    p.first() = 4;
    p.second() = "s";
    CHECK(4 == p.first());
    CHECK("s"sv == p.second());
    compressed_pair<none, int> p2{none{}, 3};
    CHECK(3 == p2.second());
}

using pii = compressed_pair<int, int>;

TEST_CASE("equality") {
    CHECK(pii{0, 1} == pii{0, 1});
    CHECK(pii{0, 1} != pii{0, 0});
    CHECK(pii{0, 1} != pii{1, 1});
    CHECK(pii{0, 1} != pii{1, 0});
}

TEST_CASE("order") {
    CHECK_FALSE(pii{0, 0} < pii{0, 0});
    CHECK(pii{0, 1} >= pii{0, 0});
    CHECK(pii{0, 1} <= pii{1, 0});
    CHECK(pii{1, 0} > pii{0, 1});
}

namespace {
struct fin final : none {};
} // namespace

static_assert(2 * sizeof(int) == sizeof(compressed_pair<int, int>));
// Objects of the same class can't share address.
static_assert(0 < sizeof(compressed_pair<none, none>));
static_assert(sizeof(int) == sizeof(compressed_pair<none, int>));
static_assert(sizeof(int) == sizeof(compressed_pair<int, none>));
static_assert(sizeof(int) < sizeof(compressed_pair<fin, int>));
static_assert(sizeof(int) < sizeof(compressed_pair<int, fin>));
