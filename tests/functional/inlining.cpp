// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/inlining/all.hpp>

// There's no way in C++ to test if a function was actually inlined.
// So, we only test that function definitions are compiled successfully.
// TODO: provide more meaningful examples.

AC_ALWAYS_INLINE int force_inlined() {
    return 2;
}

AC_NEVER_INLINE int never_inlined() {
    return 42;
}
