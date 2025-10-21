// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// Checks if the macro parameter is empty.
#define AC_IS_EMPTY(x) (#x[0] == '\0')
// Conversion to a string literal seems to be the easiest way.
