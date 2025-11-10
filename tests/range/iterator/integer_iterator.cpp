// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/iterator/integer_iterator.hpp>
#include <actl_test/iterator/iterator.hpp>
#include "test.hpp"

TEST_CASE("integer_iterator") {
    /* increment by 1 */
    using iter_inc = ac::integer_iterator<int>;
    static_assert(sizeof(iter_inc) == sizeof(int));
    static_assert(!std::is_constructible_v<iter_inc, int, int>);
    ac::test_random_access_iterator(iter_inc{}, iter_inc{2});
    /* decrement by 1 */
    using iter_dec = ac::integer_iterator<int, ac::constant<-1>>;
    static_assert(sizeof(iter_dec) == sizeof(int));
    static_assert(!std::is_constructible_v<iter_dec, int, int>);
    ac::test_random_access_iterator(iter_dec{2}, iter_dec{});
    /* step != 1 */
    using iter_step = ac::integer_iterator<int, int>;
    static_assert(sizeof(iter_step) == 2 * sizeof(int));
    static_assert(!std::is_constructible_v<iter_step, int>);
    ac::test_random_access_iterator(iter_step{1, 2}, iter_step{5, 2});
}
