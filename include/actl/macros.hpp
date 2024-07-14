// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/platform/compiler.hpp>

#define CAT_IMPL(x, y) x##y
#define CAT(x, y) CAT_IMPL(x, y)

#if AC_COMPILER_GCC_COMPATIBLE()

#define EXPECT_TRUE(condition) __builtin_expect((condition), true)
#define EXPECT_FALSE(condition) __builtin_expect((condition), false)

#else

#define EXPECT_TRUE(condition) (condition)
#define EXPECT_FALSE(condition) (condition)

#endif
