// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/dummy_container.hpp>
#include "test.hpp"

TEST_CASE("dummy_container basic") {
    dummy_container c(3);
    c.emplace_back(1);
    CHECK(4l == c.size());
    c.erase(0);
    c.erase(1);
    CHECK(2l == c.size());
    c = {};
    CHECK(c.empty());
    c.resize(3);
    CHECK(3l == c.size());
    CHECK_FALSE(c.empty());
}
