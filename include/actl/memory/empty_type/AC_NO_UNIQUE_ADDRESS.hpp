// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/platform/compiler.hpp>

/// Portable replacement for `[[no_unique_address]]` attribute.
#if AC_COMPILER_MSVC()
#define AC_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define AC_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
