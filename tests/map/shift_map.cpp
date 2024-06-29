// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/map/shift_map.hpp>
#include "test.hpp"

template<class Map>
void test_lowercase(Map map) {
    CHECK(1 == get(map, 'b'));
    CHECK('c' == invert(map, 2));
}

TEST_CASE("lowercase") {
    test_lowercase(make_shift_map<int>('a'));
    test_lowercase(static_shift_map<'a', int>{});
}
