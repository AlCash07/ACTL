// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/composite/all.hpp>
#include <actl/functional/policy/allow_promotion.hpp>

TEST_CASE("simple operation") {
    static_assert(3LL == math::add(math::allow_promotion{})(1, 2LL));
}

TEST_CASE("nested composite operation") {
    const std::vector<std::pair<char, long long>> lhs{{2, 3}, {5, 8}};
    const std::vector<std::pair<int, unsigned short>> rhs{{2, 3}, {5, 8}};
    CHECK(math::equal(math::allow_promotion{})(lhs, rhs));
}

TEST_CASE("nested expression operation") {
    constexpr auto sum3 = math::add + 3LL;
    static_assert(6LL == sum3(1LL, 2LL));
    static_assert(6LL == sum3(math::allow_promotion{})(short{1}, int{2}));
}
