// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/preprocessor/AC_IS_EMPTY.hpp>

static_assert(AC_IS_EMPTY());
static_assert(!AC_IS_EMPTY(0));
static_assert(!AC_IS_EMPTY(AAA));
static_assert(!AC_IS_EMPTY(()));
