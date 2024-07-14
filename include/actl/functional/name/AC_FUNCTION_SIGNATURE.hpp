// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/platform/compiler.hpp>

/// String literal representing compiler-specific signature
/// of the enclosing function.
#if AC_COMPILER_GCC_COMPATIBLE()
#define AC_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif AC_COMPILER_MSVC()
#define AC_FUNCTION_SIGNATURE __FUNCSIG__
#endif
