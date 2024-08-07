// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/iterator/traits/output_type.hpp>
#include <actl/std/deque.hpp>
#include "test.hpp"

TEST_CASE("output_type correctness") {
    CHECK(std::is_same_v<int, output_type_t<int*>>);
    CHECK(std::is_same_v<int, output_type_t<std::ostream_iterator<int>>>);
    CHECK(std::is_same_v<
          int,
          output_type_t<std::back_insert_iterator<std::deque<int>>>>);
    CHECK(std::is_same_v<
          int,
          output_type_t<std::front_insert_iterator<std::deque<int>>>>);
    CHECK(std::is_same_v<
          int,
          output_type_t<std::insert_iterator<std::deque<int>>>>);
}
