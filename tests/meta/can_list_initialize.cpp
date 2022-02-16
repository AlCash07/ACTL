// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/can_list_initialize.hpp>

struct aggregate
{
    int x;
    float y;
};

static_assert(ac::can_list_initialize_v<int, int>);
static_assert(!ac::can_list_initialize_v<int, long long>);
static_assert(ac::can_list_initialize_v<aggregate, int, float>);
static_assert(!ac::can_list_initialize_v<aggregate, int, int>);
static_assert(ac::can_list_initialize_v<int[3], int, int, int>);
static_assert(ac::can_list_initialize_v<int[3], int, int>);
static_assert(!ac::can_list_initialize_v<int[3], int, int, int, int>);
