/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/std/vector.hpp>
#include <actl/test.hpp>
#include <actl/util/type_traits.hpp>

TEST("value_t") {
    ASSERT_TRUE(std::is_same_v<const int, value_t<std::vector<const int>>>);
    ASSERT_TRUE(std::is_same_v<const int, value_t<const int[8]>>);
    ASSERT_TRUE(std::is_same_v<int, value_t<const int* const>>);
}

TEST("reference_t") {
    ASSERT_TRUE(std::is_same_v<int&, reference_t<std::vector<int>>>);
    ASSERT_TRUE(std::is_same_v<const int&, reference_t<const std::vector<int>>>);
    ASSERT_TRUE(std::is_same_v<int&, reference_t<int[8]>>);
    ASSERT_TRUE(std::is_same_v<const int&, reference_t<const int* const>>);
}

TEST("iterator_t") {
    using VI = std::vector<int>;
    ASSERT_TRUE(std::is_same_v<VI::iterator, iterator_t<VI>>);
    ASSERT_TRUE(std::is_same_v<VI::const_iterator, iterator_t<const VI>>);
    ASSERT_TRUE(std::is_same_v<int*, iterator_t<int[8]>>);
    ASSERT_TRUE(std::is_same_v<const int*, iterator_t<int const[8]>>);
}

TEST("add_const_if") {
    ASSERT_TRUE(std::is_same_v<int, add_const_if_t<false, int>>);
    ASSERT_TRUE(std::is_same_v<const int, add_const_if_t<true, int>>);
}

TEST("remove_cvref") { ASSERT_TRUE(std::is_same_v<int, remove_cvref_t<volatile const int&>>); }

TEST("nth_type") {
    ASSERT_TRUE(std::is_same_v<int, nth_t<0, int, float>>);
    ASSERT_TRUE(std::is_same_v<float, nth_t<1, int, float>>);
}
