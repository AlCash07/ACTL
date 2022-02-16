// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/from_range_to_tuple.hpp>
#include "test.hpp"

TEST_CASE("conversion from vector to tuple")
{
    using vec_i = std::vector<int>;
    using tuple_iii = std::tuple<int, int, int>;
    static_assert(!std::is_constructible_v<tuple_iii, vec_i>);
    static_assert(ac::can_convert_to_v<tuple_iii, vec_i>);
    CHECK(tuple_iii{5, 4, 2} == ac::convert_to<tuple_iii>(vec_i{5, 4, 2}));
    using tuple_ii = std::tuple<int, int>;
    static_assert(ac::can_convert_to_v<tuple_ii, vec_i>);
    CHECK_THROWS(ac::convert_to<tuple_ii>(vec_i{5, 4, 2}));
}
