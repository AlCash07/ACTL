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
    std::vector<std::string> s{"a", "s", "p"};
    std::vector<bool> used(3);
    for (auto[key, value] : apm) {
        ASSERT_TRUE(0 <= value && value < 3 && !used[value]);
        used[value] = true;
        ASSERT_EQUAL(s[value], key);
    }
    ASSERT_EQUAL(std::vector<bool>{true, true, true}, used);
    if constexpr (Invertible) {
        for (int i = 0; i < 3; ++i) {
            ASSERT_EQUAL(s[i], apm.invert(i));
        }
    }
}

TEST("accounting_property_map::non-invertible") { test_accounting_pm<false>(); }

TEST("accounting_property_map::invertible") { test_accounting_pm<true>(); }
