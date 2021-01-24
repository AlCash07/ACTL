// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#ifndef ACTL_ASSERT
#ifdef ACTL_DEBUG

#include <stdexcept>
#include <string>

#ifdef _MSC_VER
#define FUNCTION_NAME __FUNCSIG__
#else
#define FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#define ACTL_ASSERT(condition)                                                             \
    if (!(condition)) {                                                                    \
        throw std::runtime_error(                                                          \
            std::string{} + #condition + " in line " + std::to_string(__LINE__) + " in " + \
            FUNCTION_NAME);                                                                \
    }

#else

#define ACTL_ASSERT(condition) ((void)0)

#endif
#endif
