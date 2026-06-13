// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/platform/compiler.hpp>

#if AC_COMPILER_MSVC()
#define AC_PRAGMA(t) __pragma(t)
#else
#define AC_PRAGMA(t) _Pragma(#t)
#endif
