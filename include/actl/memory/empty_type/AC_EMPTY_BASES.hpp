// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/platform/compiler.hpp>

/// Workaround to make EBO work as expected on MSVC with multiple inheritance:
/// @code
/// struct AC_EMPTY_BASES derived : empty1, empty2 {
///     int i;
/// };
/// static_assert(sizeof(derived) == sizeof(int));
/// @endcode
#if AC_COMPILER_MSVC()
#define AC_EMPTY_BASES __declspec(empty_bases)
#else
#define AC_EMPTY_BASES
#endif
