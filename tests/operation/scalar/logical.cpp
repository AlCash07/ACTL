// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/operation/scalar/logical/all.hpp>

static_assert(true == ac::logical_not(false));
static_assert(false == ac::logical_not(true));
static_assert(true == ac::logical_and(true, true));
static_assert(false == ac::logical_and(true, false));
static_assert(true == ac::logical_or(true, false));
static_assert(false == ac::logical_or(false, false));
static_assert(true == ac::logical_implies(true, true));
static_assert(true == ac::logical_implies(false, true));
static_assert(false == ac::logical_implies(true, false));
