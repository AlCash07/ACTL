// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/equality/range_tuple.hpp>
#include <vector>
#include "test.hpp"

template<bool Result, typename L, typename R>
void test_is_equal_sequence(L const& l, R const& r) {
    CHECK(Result == ac::is_equal_sequence(l, r));
    CHECK(Result == ac::is_equal_sequence(r, l));
}

TEST_CASE("is_equal_sequence for tuple/vector combination") {
    test_is_equal_sequence<true>(
        std::tuple{4, 2}, std::vector<long long>{4, 2}
    );
    /* size mismatch with a common prefix */ {
        test_is_equal_sequence<false>(std::tuple{4, 2}, std::vector{4});
        test_is_equal_sequence<false>(std::tuple{4}, std::vector{4, 2});
    }
    /* element mismatch */ {
        test_is_equal_sequence<false>(std::tuple{4, 2}, std::vector{2, 2});
    }
}
