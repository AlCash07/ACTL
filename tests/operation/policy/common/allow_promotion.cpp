// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/operation/policy/common/allow_promotion.hpp>
#include <actl/operation/tuple/equal_tuple.hpp>
#include <actl/range/operation/equal_range.hpp>
#include "test.hpp"

// simple operation
static_assert(3LL == (ac::add | ac::allow_promotion{})(1, 2LL));

TEST_CASE("nested composite operation") {
    std::vector<std::pair<char, long long>> const l{{char{2}, 3}, {char{5}, 8}};
    std::vector<std::pair<int, uint16_t>> const r{
        {2, uint16_t{3}}, {5, uint16_t{8}}
    };
    CHECK((ac::equal | ac::allow_promotion{})(l, r));
}

// nested expression operation
constexpr auto sum3 = ac::add + 3LL;
static_assert(6LL == sum3(1LL, 2LL));
static_assert(6LL == (sum3 | ac::allow_promotion{})(short{1}, int{2}));
