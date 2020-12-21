// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/container/hash_set.hpp>
#include <actl/std/tuple.hpp>
#include <actl/util/rebind.hpp>

struct A {};
struct B {};

template <class T>
struct R {};

namespace ac {

template <class T, class U>
struct rebind<R<T>, U> {
    using type = A;
};

}  // namespace ac

TEST_CASE("specialized_rebind") {
    CHECK(std::is_same_v<A, rebind_t<R<A>, B>>);
}

TEST_CASE("inner") {
    CHECK(std::is_same_v<std::tuple<B, B, A, R<B>>, rebind_t<std::tuple<A, A, R<A>, R<B>>, B>>);
}

TEST_CASE("rebind array") {
    CHECK(std::is_same_v<B[2], rebind_t<A[2], B>>);
}

TEST_CASE("rebind hash_set") {
    using HSF = hash_set<float, hash_function<float>, std::equal_to<float>>;
    CHECK(std::is_same_v<hash_set<int>, rebind_t<HSF, int>>);
}
