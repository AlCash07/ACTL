// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/common.hpp>

TEST_CASE("math::common") {
    using one = std::integral_constant<int, 1>;
    static_assert(std::is_same_v<none, decltype(eval(math::common(none{}, none{}, none{})))>);
    static_assert(1 == eval(math::common(none{}, one{}, none{})).value);
    static_assert(1 == eval(math::common(one{})).value);
    static_assert(1 == eval(math::common(one{}, one{}, one{})).value);
    int x = 1;
    static_assert(std::is_same_v<one, decltype(eval(math::common(x, one{}, x)))>);
    CHECK(2 == math::common(2, 2, 2));
}
