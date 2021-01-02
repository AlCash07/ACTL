// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/iterator/traits.hpp>
#include <actl/std/forward_list.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>
#include <iterator>

TEST_CASE("is_iterator") {
    CHECK_FALSE(is_iterator_v<int[4]>);
    CHECK(is_iterator_v<int*>);
    CHECK(is_iterator_v<std::vector<int>::iterator>);
}

TEST_CASE("is_const_iterator") {
    CHECK_FALSE(is_const_iterator_v<int[4]>);
    CHECK_FALSE(is_const_iterator_v<int*>);
    CHECK(is_const_iterator_v<std::vector<int>::const_iterator>);
}

TEST_CASE("is_non_const_iterator") {
    CHECK_FALSE(is_non_const_iterator_v<int[4]>);
    CHECK_FALSE(is_non_const_iterator_v<const int*>);
    CHECK(is_non_const_iterator_v<std::vector<int>::iterator>);
}

TEST_CASE("C array is not any kind of iterator") {
    using Iter = int[4];
    CHECK_FALSE(is_input_iterator_v<Iter>);
    CHECK_FALSE(is_output_iterator_v<Iter>);
    CHECK_FALSE(is_forward_iterator_v<Iter>);
    CHECK_FALSE(is_bidirectional_iterator_v<Iter>);
    CHECK_FALSE(is_random_access_iterator_v<Iter>);
}

TEST_CASE("std::istream_iterator is only input iterator") {
    using Iter = std::istream_iterator<int>;
    CHECK_FALSE(is_output_iterator_v<Iter>);
    CHECK(is_input_iterator_v<Iter>);
    CHECK_FALSE(is_forward_iterator_v<Iter>);
}

TEST_CASE("std::ostream_iterator is only output iterator") {
    using Iter = std::ostream_iterator<int>;
    CHECK_FALSE(is_input_iterator_v<Iter>);
    CHECK(is_output_iterator_v<Iter>);
    CHECK_FALSE(is_forward_iterator_v<Iter>);
}

TEST_CASE("std::forward_list iterator is forward") {
    using Iter = std::forward_list<int>::iterator;
    CHECK_FALSE(is_output_iterator_v<Iter>);
    CHECK(is_forward_iterator_v<Iter>);
    CHECK_FALSE(is_random_access_iterator_v<Iter>);
}

TEST_CASE("std::list iterator is bidirectional") {
    using Iter = std::list<int>::iterator;
    CHECK_FALSE(is_output_iterator_v<Iter>);
    CHECK(is_forward_iterator_v<Iter>);
    CHECK(is_bidirectional_iterator_v<Iter>);
    CHECK_FALSE(is_random_access_iterator_v<Iter>);
}

TEST_CASE("std::vector iterator is random access") {
    using Iter = std::vector<int>::iterator;
    CHECK_FALSE(is_output_iterator_v<Iter>);
    CHECK(is_forward_iterator_v<Iter>);
    CHECK(is_bidirectional_iterator_v<Iter>);
    CHECK(is_random_access_iterator_v<Iter>);
}
