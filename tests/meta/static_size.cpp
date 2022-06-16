// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/static_size.hpp>
#include <utility>

static_assert(3 == ac::static_size_v<int[3]>);
static_assert(2 == ac::static_size_v<std::pair<int, float>>);
/* reference */
static_assert(3 == ac::static_size_v<int (&)[3]>);
/* const type */
static_assert(2 == ac::static_size_v<std::pair<int, float> const>);

static_assert(ac::dynamic_size == ac::static_size_v<int>);

struct S
{};
static_assert(ac::dynamic_size == ac::static_size_v<S>);
static_assert(ac::dynamic_size == ac::static_size_v<S const>);

/* static_sizes_match */
static_assert(ac::static_sizes_match(2, 2));
static_assert(!ac::static_sizes_match(2, 3));
static_assert(ac::static_sizes_match(2, ac::dynamic_size));
static_assert(ac::static_sizes_match(ac::dynamic_size, 2));
static_assert(ac::static_sizes_match(ac::dynamic_size, ac::dynamic_size));

/* have_matching_static_sizes */
static_assert(ac::have_matching_static_sizes_v<int[2], std::pair<int, int>>);
static_assert(!ac::have_matching_static_sizes_v<int[2], int[3]>);
