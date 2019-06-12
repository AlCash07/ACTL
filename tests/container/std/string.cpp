/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/string.hpp>
#include <actl/test.hpp>

TEST("category") {
    using C = std::string;
    ASSERT_TRUE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_random_access_container_v<C>);
    ASSERT_TRUE(is_contiguous_container_v<C>);
    ASSERT_FALSE(is_associative_container_v<C>);
}

using namespace ac::io;

TEST("read") {
    std::string s = "  01234567890123456789 0123456789";
    std::string x;
    ASSERT_TRUE(read(formatted<memory<in>>{s}, x));
    ASSERT_EQUAL("01234567890123456789", x);
}
