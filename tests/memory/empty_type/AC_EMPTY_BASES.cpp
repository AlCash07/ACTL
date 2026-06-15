// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/memory/empty_type/AC_EMPTY_BASES.hpp>

struct empty1 {};
struct empty2 {};

struct AC_EMPTY_BASES derived
    : empty1
    , empty2 {
    int i;
};
static_assert(sizeof(derived) == sizeof(int));

struct derived_wrong
    : empty1
    , empty2 {
    int i;
};

#if AC_COMPILER_MSVC()
static_assert(sizeof(derived_wrong) > sizeof(int));
#endif
