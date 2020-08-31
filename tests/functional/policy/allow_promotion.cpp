/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/policy/allow_promotion.hpp>
#include <actl/functional/composite/all.hpp>
#include <actl/test.hpp>

TEST("simple operation") {
    static_assert(3LL == math::add(math::allow_promotion{})(1, 2LL));
}

TEST("nested composite operation") {
    const std::vector<std::pair<char, long long>> lhs{{2, 3}, {5, 8}};
    const std::vector<std::pair<int, unsigned short>> rhs{{2, 3}, {5, 8}};
    ASSERT_TRUE(math::equal(math::allow_promotion{})(lhs, rhs));
}
