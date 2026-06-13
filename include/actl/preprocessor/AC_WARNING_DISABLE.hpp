// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/preprocessor/AC_PRAGMA.hpp>

#define AC_WARNING_DISABLE(WARNING) AC_WARNING_DISABLE_IMPL(WARNING)
#define AC_WARNING_ENABLE() AC_WARNING_ENABLE_IMPL()

#if AC_COMPILER_MSVC()

#define AC_WARNING_DISABLE_IMPL(WARNING) \
    AC_PRAGMA(warning(push))             \
    AC_PRAGMA(warning(disable : WARNING))

#define AC_WARNING_ENABLE_IMPL() AC_PRAGMA(warning(pop))

#else

#define AC_WARNING_DISABLE_IMPL(WARNING) \
    AC_PRAGMA(GCC diagnostic push)       \
    AC_PRAGMA(GCC diagnostic ignored #WARNING)

#define AC_WARNING_ENABLE_IMPL() AC_PRAGMA(GCC diagnostic pop)

#endif

// clang-format off
#if AC_COMPILER_MSVC()

#define AC_UNDEFINED_FUNCTION 4506

#else

#define AC_UNDEFINED_FUNCTION -Wundefined-inline

#endif
// clang-format on
