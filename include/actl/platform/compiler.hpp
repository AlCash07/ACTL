// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/* Compiler detection macros */

#define AC_COMPILER_GCC() 0
#define AC_COMPILER_CLANG() 0
#define AC_COMPILER_MSVC() 0

// We need to check for clang before gcc,
// because clang is gcc-compatible and also defines __GNUC__.
#if defined(__clang__)
#undef AC_COMPILER_CLANG
#define AC_COMPILER_CLANG() 1
#elif defined(__GNUC__)
#undef AC_COMPILER_GCC
#define AC_COMPILER_GCC() 1
#elif defined(_MSC_VER)
#undef AC_COMPILER_MSVC
#define AC_COMPILER_MSVC() 1
#endif

#define AC_COMPILER_GCC_COMPATIBLE() (AC_COMPILER_GCC() || AC_COMPILER_CLANG())
