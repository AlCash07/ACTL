/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/accounting_map.hpp>
#include <actl/std/map.hpp>
#include <actl/test.hpp>

template <bool Invertible>
inline void test_accounting_map() {
    accounting_map<std::map<std::string, int>, Invertible> map;
    ASSERT_EQUAL(0, get(map, "a"));
    ASSERT_EQUAL(1, get(map, "s"));
    ASSERT_EQUAL(0, get(map, "a"));
    ASSERT_EQUAL(2, get(map, "p"));
    ASSERT_EQUAL(1, get(map, "s"));
    using C = std::vector<std::pair<std::string, int>>;
    C expected{{"a", 0}, {"s", 1}, {"p", 2}};
    auto r = map_range(map);
    ASSERT_EQUAL_SETS(expected, {r.begin(), r.end()});
    if constexpr (Invertible) {
        for (int i = 0; i < 3; ++i) {
            ASSERT_EQUAL(expected[(unsigned)i].first, invert(map, i));
        }
    }
}

TEST("non-invertible") { test_accounting_map<false>(); }

TEST("invertible") { test_accounting_map<true>(); }
