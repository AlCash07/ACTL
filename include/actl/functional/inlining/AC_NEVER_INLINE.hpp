// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/platform/compiler.hpp>

/// Macro to prevent inlining of a function.
#if AC_COMPILER_GCC_COMPATIBLE()
#define AC_NEVER_INLINE __attribute__((noinline))
#elif AC_COMPILER_MSVC()
#define AC_NEVER_INLINE __declspec(noinline)
#endif
