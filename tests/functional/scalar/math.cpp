// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/math.hpp>
#include <limits>

TEST_CASE("abs") {
    CHECK(0 == math::abs(0));
    constexpr auto int_max = std::numeric_limits<int>::max();
    CHECK(int_max == math::abs(int_max));
    CHECK(int_max == math::abs(-int_max));
    constexpr auto uint_max = std::numeric_limits<unsigned int>::max();
    CHECK(uint_max == math::abs(uint_max));
}
