// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/to_range.hpp>
#include <actl/container/equality/arrays.hpp>
#include <array>
#include <vector>
#include "test.hpp"

/* conversion to a fixed-size range */
using Array2i = std::array<int, 2>;
// same types
static_assert(ac::can_convert_to_v<Array2i, int, int>);
static_assert(ac::is_equal_array(Array2i{3, 2}, ac::convert_to<Array2i>(3, 2)));
// compatible types
static_assert(ac::can_convert_to_v<Array2i, uint32_t, int>);
static_assert(
    ac::is_equal_array(Array2i{3, 2}, ac::convert_to<Array2i>(uint32_t{3}, 2))
);
// different size
static_assert(!ac::can_convert_to_v<Array2i, int>);
static_assert(!ac::can_convert_to_v<Array2i, int, int, int>);
// incompatible types
static_assert(!ac::can_convert_to_v<Array2i, void*, int>);

TEST_CASE("conversion to a dynamic-size range") {
    // same types
    static_assert(ac::can_convert_to_v<std::vector<int>, int, int, int>);
    CHECK(
        std::vector<int>{3, 2, 1} == ac::convert_to<std::vector<int>>(3, 2, 1)
    );
    // compatible types
    static_assert(ac::can_convert_to_v<std::vector<uint32_t>, int, int>);
    CHECK(
        std::vector<uint32_t>{3, 2} ==
        ac::convert_to<std::vector<uint32_t>>(3, 2)
    );
    // incompatible types
    static_assert(!ac::can_convert_to_v<std::vector<int>, int*, int>);
}
