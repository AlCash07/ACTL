// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/map/accounting_map.hpp>
#include <actl/std/map.hpp>
#include "test.hpp"

template <bool Invertible>
void test_accounting_map()
{
    accounting_map<std::map<std::string, int>, Invertible> map;
    CHECK(0 == get(map, "a"));
    CHECK(1 == get(map, "s"));
    CHECK(0 == get(map, "a"));
    CHECK(2 == get(map, "p"));
    CHECK(1 == get(map, "s"));
    using C = std::vector<std::pair<std::string, int>>;
    C expected{{"a", 0}, {"s", 1}, {"p", 2}};
    auto r = map_range(map);
    CHECK_EQUAL_SETS(expected, {r.begin(), r.end()});
    if constexpr (Invertible)
    {
        for (int i = 0; i < 3; ++i)
        {
            CHECK(expected[(unsigned)i].first == invert(map, i));
        }
    }
}

TEST_CASE("non-invertible")
{
    test_accounting_map<false>();
}

TEST_CASE("invertible")
{
    test_accounting_map<true>();
}
