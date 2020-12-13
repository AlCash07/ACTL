// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/io/device/memory.hpp>
#include <actl/io/util/till.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("getline") {
    std::string s = "line\nnext";
    memory<bin | in> id{s};
    char x[9] = {};
    ASSERT_TRUE(read(id, till{span{x}, '\n'}));
    ASSERT_EQUAL("line", span{x, 5});
    id.move(1);
    ASSERT_TRUE(read(id, till{span{x}, '\n'}));
    ASSERT_EQUAL("next", span{x, 5});
}
