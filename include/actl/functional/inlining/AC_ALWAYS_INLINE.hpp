// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/platform/compiler.hpp>

/// Macro to force inlining of a function.
///
/// Should be used with caution to avoid undesirable binary size increase.
///
/// @note It's not recommended to overuse it, because
/// it disables stepping into the function in debug mode on some platforms.
#if AC_COMPILER_GCC_COMPATIBLE()
#define AC_ALWAYS_INLINE __attribute__((__always_inline__))
#elif AC_COMPILER_MSVC()
#define AC_ALWAYS_INLINE __forceinline
#endif
