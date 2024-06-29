// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/hash_set.hpp>
#include <actl/meta/rebind.hpp>
#include <actl/std/tuple.hpp>
#include "test.hpp"

namespace {

template <class T>
struct R {};

struct A {};

struct B {};

} // namespace

namespace ac {

template <class T, class U>
struct rebind<R<T>, U> {
    using type = int;
};

} // namespace ac

// Specialization has the highest priority.
static_assert(std::is_same_v<int, ac::rebind_t<R<A>, B>>);

static_assert(std::is_same_v<
              std::tuple<B, B, int, R<B>>,
              ac::rebind_t<std::tuple<A, A, R<A>, R<B>>, B>>);

static_assert(std::is_same_v<B[2], ac::rebind_t<A[2], B>>);

using HSF = hash_set<float, hash_function<float>, std::equal_to<float>>;
static_assert(std::is_same_v<hash_set<int>, ac::rebind_t<HSF, int>>);
