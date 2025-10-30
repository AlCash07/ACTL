// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/equal_sequences/range_tuple.hpp>
#include <vector>
#include "test.hpp"

template<bool Result, typename L, typename R>
void test_equal_sequences(L const& l, R const& r) {
    CHECK(Result == ac::equal_sequences(l, r));
    CHECK(Result == ac::equal_sequences(r, l));
}

TEST_CASE("equal_sequences for tuple/vector combination") {
    test_equal_sequences<true>(std::tuple{4, 2}, std::vector<long long>{4, 2});
    /* size mismatch with a common prefix */ {
        test_equal_sequences<false>(std::tuple{4, 2}, std::vector{4});
        test_equal_sequences<false>(std::tuple{4}, std::vector{4, 2});
    }
    /* element mismatch */ {
        test_equal_sequences<false>(std::tuple{4, 2}, std::vector{2, 2});
    }
}
