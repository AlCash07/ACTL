// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/platform/compiler.hpp>

#ifndef AC_ASSERT
#ifdef ACTL_DEBUG

#include <stdexcept>
#include <string>

#if AC_COMPILER_MSVC()
#define FUNCTION_NAME __FUNCSIG__
#else
#define FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#define AC_ASSERT(condition)                                  \
    if (!(condition)) {                                       \
        throw std::runtime_error(                             \
            std::string{} + #condition + " in line " +        \
            std::to_string(__LINE__) + " in " + FUNCTION_NAME \
        );                                                    \
    }

#else

#define AC_ASSERT(condition) ((void)0)

#endif
#endif

#ifdef ACTL_DEBUG
#define AC_ASSERT_IS_NOEXCEPT() 0
#else
#define AC_ASSERT_IS_NOEXCEPT() 1
#endif
