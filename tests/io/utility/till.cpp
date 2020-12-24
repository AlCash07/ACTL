// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/io/device/memory.hpp>
#include <actl/io/utility/till.hpp>

using namespace ac::io;

TEST_CASE("getline") {
    std::string s = "line\nnext";
    memory<bin | in> id{s};
    char x[9] = {};
    CHECK(read(id, till{span{x}, '\n'}));
    CHECK_EQUAL("line", span{x, 5});
    id.move(1);
    CHECK(read(id, till{span{x}, '\n'}));
    CHECK_EQUAL("next", span{x, 5});
}
