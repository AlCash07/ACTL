// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/map/shift_map.hpp>
#include <actl/test.hpp>

template <class Map>
void test_lowercase(Map map) {
    ASSERT_EQUAL(1, get(map, 'b'));
    ASSERT_EQUAL('c', invert(map, 2));
}

TEST("lowercase") {
    test_lowercase(make_shift_map<int>('a'));
    test_lowercase(static_shift_map<'a', int>{});
}
