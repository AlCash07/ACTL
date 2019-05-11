/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/device/string.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/test.hpp>

template <class... Ts>
void test_output(const std::string& expected, Ts&&... xs) {
    std::string s;
    write(io::string<io::app>{s}, std::forward<Ts>(xs)...);
    ASSERT_EQUAL(expected, s);
}
