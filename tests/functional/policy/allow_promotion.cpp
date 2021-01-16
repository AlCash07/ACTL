// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/policy/allow_promotion.hpp>
#include <actl/functional/range/equal_range.hpp>
#include <actl/functional/tuple/equal_tuple.hpp>
#include "test.hpp"

TEST_CASE("simple operation") {
    STATIC_ASSERT_WORKAROUND(3LL == (ac::add | ac::allow_promotion{})(1, 2LL));
}

TEST_CASE("nested composite operation") {
    const std::vector<std::pair<char, long long>> lhs{{char{2}, 3}, {char{5}, 8}};
    const std::vector<std::pair<int, uint16_t>> rhs{{2, uint16_t{3}}, {5, uint16_t{8}}};
    CHECK((ac::equal | ac::allow_promotion{})(lhs, rhs));
}

TEST_CASE("nested expression operation") {
    constexpr auto sum3 = ac::add + 3LL;
    STATIC_ASSERT_WORKAROUND(6LL == sum3(1LL, 2LL));
    STATIC_ASSERT_WORKAROUND(6LL == (sum3 | ac::allow_promotion{})(short{1}, int{2}));
}
