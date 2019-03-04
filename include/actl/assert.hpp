/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#ifndef ACTL_ASSERT
#ifdef ACTL_DEBUG

#include <stdexcept>
#include <string>

#define ACTL_ASSERT_IMPL(condition, function)                                   \
    if (!(condition)) {                                                         \
        throw std::runtime_error(std::string() + #condition + " in line " +     \
                                 std::to_string(__LINE__) + " in " + function); \
    }

#ifdef _MSC_VER
#define ACTL_ASSERT(condition) ACTL_ASSERT_IMPL(condition, __FUNCSIG__)
#else
#define ACTL_ASSERT(condition) ACTL_ASSERT_IMPL(condition, __PRETTY_FUNCTION__)
#endif
#undef ACTL_ASSERT_IMPL

#else

#define ACTL_ASSERT(condition) ((void)0)

#endif
#endif
