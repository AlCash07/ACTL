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
inline void test_accounting_pm() {
    accounting_map<std::map<std::string, int>, Invertible> pm;
    ASSERT_EQUAL(0, get(pm, "a"));
    ASSERT_EQUAL(1, get(pm, "s"));
    ASSERT_EQUAL(0, get(pm, "a"));
    ASSERT_EQUAL(2, get(pm, "p"));
    ASSERT_EQUAL(1, get(pm, "s"));
    using C = std::vector<std::pair<std::string, int>>;
    C expected{{"a", 0}, {"s", 1}, {"p", 2}};
    ASSERT_EQUAL_SETS(expected, {pm.begin(), pm.end()});
    if constexpr (Invertible) {
        for (int i = 0; i < 3; ++i) {
            ASSERT_EQUAL(expected[(unsigned)i].first, pm.invert(i));
        }
    }
}

TEST("non-invertible") { test_accounting_pm<false>(); }

TEST("invertible") { test_accounting_pm<true>(); }
