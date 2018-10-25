/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/accounting_property_map.hpp>
#include <actl/test.hpp>
#include <map>

using namespace ac;

template <bool Invertible>
inline void test_accounting_pm() {
    accounting_property_map<std::map<std::string, int>, Invertible> apm;
    ASSERT_EQUAL(0, get(apm, "a"));
    ASSERT_EQUAL(1, get(apm, "s"));
    ASSERT_EQUAL(0, get(apm, "a"));
    ASSERT_EQUAL(2, get(apm, "p"));
    ASSERT_EQUAL(1, get(apm, "s"));
    using C = std::vector<std::pair<std::string, int>>;
    C expected{{"a", 0}, {"s", 1}, {"p", 2}};
    ASSERT_EQUAL_SETS(expected, {apm.begin(), apm.end()});
    if constexpr (Invertible) {
        for (int i = 0; i < 3; ++i) {
            ASSERT_EQUAL(expected[i].first, apm.invert(i));
        }
    }
}

TEST("non-invertible") { test_accounting_pm<false>(); }

TEST("invertible") { test_accounting_pm<true>(); }
