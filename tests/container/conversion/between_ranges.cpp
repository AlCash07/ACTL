// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/between_ranges.hpp>
#include <actl/meta/can_list_initialize.hpp>
#include <array>
#include <vector>
#include "test.hpp"

TEST_CASE("conversion between ranges") {
    using vec_i = std::vector<int>;
    SECTION("vectors with different element types") {
        using vec_ll = std::vector<long long>;
        static_assert(!std::is_constructible_v<vec_ll, vec_i>);
        static_assert(!ac::can_list_initialize_v<vec_ll, vec_i>);
        static_assert(ac::can_convert_to_v<vec_ll, vec_i>);
        CHECK(vec_ll{4, 2} == ac::convert_to<vec_ll>(vec_i{4, 2}));
    }
    SECTION("conversion into an array") {
        using array2i = std::array<int, 2>;
        static_assert(!std::is_constructible_v<vec_i, array2i>);
        static_assert(ac::can_convert_to_v<array2i, vec_i>);
        CHECK(array2i{4, 2} == ac::convert_to<array2i>(vec_i{4, 2}));
        /* different static sizes */
        static_assert(!ac::can_convert_to_v<array2i, std::array<int, 3>>);
    }
}
