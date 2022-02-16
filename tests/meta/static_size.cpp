// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/static_size.hpp>
#include <utility>

static_assert(3 == ac::static_size_v<int[3]>);
static_assert(2 == ac::static_size_v<std::pair<int, float>>);
/* const type */
static_assert(2 == ac::static_size_v<const std::pair<int, float>>);

static_assert(ac::dynamic_size == ac::static_size_v<int>);

struct S
{};
static_assert(ac::dynamic_size == ac::static_size_v<S>);
static_assert(ac::dynamic_size == ac::static_size_v<const S>);
