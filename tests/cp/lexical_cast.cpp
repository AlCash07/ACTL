/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/cp/lexical_cast.hpp>
#include <actl/test.hpp>

TEST("cp::lexical_cast") {
    ASSERT_EQUAL(1, lexical_cast<int>("1"));
    ASSERT_EQUAL("42", lexical_cast<std::string>(42));
    ASSERT_EQUAL(1.25, lexical_cast<double>("1.2500"), 1e-15);
    ASSERT_THROWS(lexical_cast<int>("1 "));
    ASSERT_THROWS(lexical_cast<int>(""));
}
