/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/scalar/derived.hpp>
#include <actl/test.hpp>
#include <limits>

TEST("sgn") {
    static_assert(1 == math::sgn(std::numeric_limits<double>::max()));
    static_assert(-1 == math::sgn(std::numeric_limits<int>::min()));
    static_assert(1 == math::sgn(std::numeric_limits<unsigned int>::max()));
    static_assert(0 == math::sgn(0));
}

TEST("sqr") {
    static_assert(36 == math::sqr(6));
    static_assert(4 == math::sqr(-2));
}
