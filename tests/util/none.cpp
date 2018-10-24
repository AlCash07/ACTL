/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/util/none.hpp>
#include <type_traits>

using namespace ac;

TEST("is_empty") { ASSERT_TRUE(std::is_empty_v<none>); }

template <class E, class T>
inline constexpr bool test_replace_void_v = std::is_same_v<E, replace_void_t<T>>;

TEST("replace_void") {
    ASSERT_TRUE(test_replace_void_v<int, int>);
    ASSERT_TRUE(test_replace_void_v<none, void>);
    ASSERT_TRUE(test_replace_void_v<const none, const void>);
}
