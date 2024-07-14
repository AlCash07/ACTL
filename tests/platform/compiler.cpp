// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/platform/compiler.hpp>

// Exactly one of the compilers is detected.
static_assert(
    1 ==
    int{AC_COMPILER_GCC()} + int{AC_COMPILER_CLANG()} + int{AC_COMPILER_MSVC()}
);

// gcc-compatible implies gcc or clang.
static_assert(
    bool{AC_COMPILER_GCC_COMPATIBLE()} ==
    (bool{AC_COMPILER_GCC()} || bool{AC_COMPILER_CLANG()})
);
