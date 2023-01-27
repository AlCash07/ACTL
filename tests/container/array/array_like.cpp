// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/array_like.hpp>
#include <actl/container/array/semi_static_array_selector.hpp>
#include <tuple>
#include <vector>

static_assert(ac::ArrayLike<ac::static_array<uint64_t, 3, 4>>);
static_assert(ac::ArrayLike<ac::semi_static_array<int32_t, -1, 4>>);
static_assert(ac::ArrayLike<std::array<uint16_t, 2>>);
static_assert(ac::ArrayLike<std::vector<int>>);
static_assert(!ac::ArrayLike<std::tuple<int, int>>);
static_assert(!ac::ArrayLike<void>);
