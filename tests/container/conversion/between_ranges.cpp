// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/between_ranges.hpp>
#include <vector>
#include "test.hpp"

TEST_CASE("conversion between vectors with different element types")
{
    using vec_i = std::vector<int>;
    using vec_ll = std::vector<long long>;
    static_assert(!std::is_constructible_v<vec_ll, vec_i>);
    static_assert(!ac::can_list_initialize_v<vec_ll, vec_i>);
    static_assert(ac::can_convert_to_v<vec_ll, vec_i>);
    CHECK(vec_ll{4, 2} == ac::convert_to<vec_ll>(vec_i{4, 2}));
}
