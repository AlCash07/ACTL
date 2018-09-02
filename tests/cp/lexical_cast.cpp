/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/cp/lexical_cast.hpp>
#include <actl/cp/test.hpp>

using namespace ac;

TEST("diagnostics::detail::to_string") {
    ASSERT_EQUAL(1, lexical_cast<int>("1"));
    ASSERT_EQUAL("42", lexical_cast<std::string>(42));
    ASSERT_EQUAL(1.25, lexical_cast<double>("1.2500"));
    ASSERT_THROWS(lexical_cast<int>("1 "));
    ASSERT_THROWS(lexical_cast<int>(""));
}
