/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/shift_property_map.hpp>
#include <actl/test.hpp>

using namespace ac;

template <class PM>
inline void test_lowercase(PM pm) {
    ASSERT_EQUAL(1, get(pm, 'b'));
    ASSERT_EQUAL('c', pm.invert(2));
}

TEST("lowercase") {
    test_lowercase(make_shift_property_map<int>('a'));
    test_lowercase(static_shift_property_map<'a', int>{});
}
