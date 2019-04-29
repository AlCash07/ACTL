/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/traits/rebind.hpp>
#include <tuple>
#include <unordered_set>

struct A {};
struct B {};

template <class T>
struct R {
    template <class U>
    using rebind = A;
};

TEST("member_rebind") { ASSERT_TRUE(std::is_same_v<A, rebind_t<R<A>, B>>); }

TEST("inner") {
    ASSERT_TRUE(
        std::is_same_v<std::tuple<B, B, A, R<B>>, rebind_t<std::tuple<A, A, R<A>, R<B>>, B>>);
}

TEST("array") { ASSERT_TRUE(std::is_same_v<B[2], rebind_t<A[2], B>>); }

TEST("unordered_set") {
    ASSERT_TRUE(std::is_same_v<std::unordered_set<int>, rebind_t<std::unordered_set<float>, int>>);
}
