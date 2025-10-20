// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/type_list/prefix.hpp>
#include <type_traits>

// The implementation for length 4+ is different,
// so we need a long enough list to test it.
using FullList = ac::type_list<void, int, long, float, double>;

static_assert(std::is_same_v<ac::type_list<>, ac::prefix_t<FullList, 0>>);
static_assert(std::is_same_v<ac::type_list<void>, ac::prefix_t<FullList, 1>>);
static_assert(std::is_same_v<
              ac::type_list<void, int>,
              ac::prefix_t<FullList, 2>>);
static_assert(std::is_same_v<
              ac::type_list<void, int, long>,
              ac::prefix_t<FullList, 3>>);
static_assert(std::is_same_v<
              ac::type_list<void, int, long, float>,
              ac::prefix_t<FullList, 4>>);
static_assert(std::is_same_v<FullList, ac::prefix_t<FullList, 5>>);
