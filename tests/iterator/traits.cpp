// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/iterator/traits.hpp>
#include <actl/std/forward_list.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>
#include <iterator>

TEST("is_iterator") {
    ASSERT_FALSE(is_iterator_v<int[4]>);
    ASSERT_TRUE(is_iterator_v<int*>);
    ASSERT_TRUE(is_iterator_v<std::vector<int>::iterator>);
}

TEST("is_const_iterator") {
    ASSERT_FALSE(is_const_iterator_v<int[4]>);
    ASSERT_FALSE(is_const_iterator_v<int*>);
    ASSERT_TRUE(is_const_iterator_v<std::vector<int>::const_iterator>);
}

TEST("is_non_const_iterator") {
    ASSERT_FALSE(is_non_const_iterator_v<int[4]>);
    ASSERT_FALSE(is_non_const_iterator_v<const int*>);
    ASSERT_TRUE(is_non_const_iterator_v<std::vector<int>::iterator>);
}

TEST("C array is not any kind of iterator") {
    using It = int[4];
    ASSERT_FALSE(is_input_iterator_v<It>);
    ASSERT_FALSE(is_output_iterator_v<It>);
    ASSERT_FALSE(is_forward_iterator_v<It>);
    ASSERT_FALSE(is_bidirectional_iterator_v<It>);
    ASSERT_FALSE(is_random_access_iterator_v<It>);
}

TEST("std::istream_iterator is only input iterator") {
    using It = std::istream_iterator<int>;
    ASSERT_FALSE(is_output_iterator_v<It>);
    ASSERT_TRUE(is_input_iterator_v<It>);
    ASSERT_FALSE(is_forward_iterator_v<It>);
}

TEST("std::ostream_iterator is only output iterator") {
    using It = std::ostream_iterator<int>;
    ASSERT_FALSE(is_input_iterator_v<It>);
    ASSERT_TRUE(is_output_iterator_v<It>);
    ASSERT_FALSE(is_forward_iterator_v<It>);
}

TEST("std::forward_list iterator is forward") {
    using It = std::forward_list<int>::iterator;
    ASSERT_FALSE(is_output_iterator_v<It>);
    ASSERT_TRUE(is_forward_iterator_v<It>);
    ASSERT_FALSE(is_random_access_iterator_v<It>);
}

TEST("std::list iterator is bidirectional") {
    using It = std::list<int>::iterator;
    ASSERT_FALSE(is_output_iterator_v<It>);
    ASSERT_TRUE(is_forward_iterator_v<It>);
    ASSERT_TRUE(is_bidirectional_iterator_v<It>);
    ASSERT_FALSE(is_random_access_iterator_v<It>);
}

TEST("std::vector iterator is random access") {
    using It = std::vector<int>::iterator;
    ASSERT_FALSE(is_output_iterator_v<It>);
    ASSERT_TRUE(is_forward_iterator_v<It>);
    ASSERT_TRUE(is_bidirectional_iterator_v<It>);
    ASSERT_TRUE(is_random_access_iterator_v<It>);
}
