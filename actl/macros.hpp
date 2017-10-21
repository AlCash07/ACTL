/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#if __GNUG__

#define EXPECT_TRUE(condition) __builtin_expect((condition), true)
#define EXPECT_FALSE(condition) __builtin_expect((condition), false)

#else

#define EXPECT_TRUE(condition) (condition)
#define EXPECT_FALSE(condition) (condition)

#endif
