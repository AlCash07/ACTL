/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/cp/diagnostics/detail/to_string.hpp>
#include <actl/cp/test.hpp>

#include <set>

using namespace ac;

TEST("diagnostics::detail::to_string") {
    using diagnostics::detail::to_string;
    ASSERT_EQUAL("true", to_string(true));
    ASSERT_EQUAL("false", to_string(false));
    ASSERT_EQUAL("\"hello\"", to_string("hello"));
    ASSERT_EQUAL("[1, 2, 3]", to_string(std::vector<int>{1, 2, 3}));
    ASSERT_EQUAL("{1, 2, 3}", to_string(std::set<int>{1, 2, 3}));
    ASSERT_EQUAL("(\"hello\", 42)", to_string(std::make_tuple("hello", 42)));
}
