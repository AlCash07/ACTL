// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/operation/scalar/common/all.hpp>
#include "test.hpp"

TEST_CASE("common") {
    using one = std::integral_constant<int, 1>;
    static_assert(std::is_same_v<none, decltype(eval(ac::common(none{}, none{}, none{})))>);
    static_assert(1 == eval(ac::common(none{}, one{}, none{})).value);
    static_assert(1 == eval(ac::common(one{})).value);
    static_assert(1 == eval(ac::common(one{}, one{}, one{})).value);
    int x = 1;
    static_assert(std::is_same_v<one, decltype(eval(ac::common(x, one{}, x)))>);
    CHECK(2 == ac::common(2, 2, 2));
}
