// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/map/dummy_map.hpp>

TEST_CASE("dummy_map basic") {
    dummy_map map;
    put(map, 2, 2);
}
