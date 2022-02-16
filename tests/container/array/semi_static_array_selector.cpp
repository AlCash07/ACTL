// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array_selector.hpp>

static_assert(
    std::is_same_v<ac::static_array<size_t>, ac::semi_static_array_t<size_t>>);
static_assert(std::is_same_v<
              ac::static_array<size_t, 1>,
              ac::semi_static_array_t<size_t, 1>>);
static_assert(std::is_same_v<
              std::array<size_t, 1>,
              ac::semi_static_array_t<size_t, ac::dynamic_extent<>>>);

static_assert(std::is_same_v<
              ac::static_array<int, 5, 3, 2>,
              ac::semi_static_array_t<int, 5, 3, 2>>);
static_assert(std::is_same_v<
              ac::semi_static_array<int, -1, 3, 2>,
              ac::semi_static_array_t<int, -1, 3, 2>>);
static_assert(std::is_same_v<
              ac::semi_static_array<int, 5, -1, -1>,
              ac::semi_static_array_t<int, 5, -1, -1>>);
static_assert(std::is_same_v<
              std::array<int, 3>,
              ac::semi_static_array_t<int, -1, -1, -1>>);
