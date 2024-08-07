// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <type_traits>

struct empty_base {};

// Empty class can be final, which was an issue for empty base optimization.
struct empty final : empty_base {};

// Size of an object of empty class is at least 1.
static_assert(std::is_empty_v<empty>);
static_assert(sizeof(empty) >= 1);

struct with_unique {
    int i;
    empty e;
};

struct without_unique {
    int i;
    AC_NO_UNIQUE_ADDRESS empty e;
};

// Empty field takes at least one more byte to have unique address.
static_assert(sizeof(with_unique) >= sizeof(int) + 1);
// But it's optimized out with AC_NO_UNIQUE_ADDRESS.
static_assert(sizeof(without_unique) == sizeof(int));

struct with_same_empty {
    int i;
    AC_NO_UNIQUE_ADDRESS empty e1, e2;
};

// e1 and e2 cannot share the same address because they have the
// same type, even though they are marked with AC_NO_UNIQUE_ADDRESS.
// However, either may share address with i.
static_assert(sizeof(with_same_empty) >= sizeof(int) + 1);

struct empty2 {};

struct with_diff_empty {
    int i;
    AC_NO_UNIQUE_ADDRESS empty e1;
    AC_NO_UNIQUE_ADDRESS empty2 e2;
};

// Different empty class object can share address.
static_assert(sizeof(with_diff_empty) == sizeof(int));

struct with_empty {
    // It's useful to apply AC_NO_UNIQUE_ADDRESS even to a single class field.
    AC_NO_UNIQUE_ADDRESS empty e;
};

struct with_nested_empty {
    int i;
    AC_NO_UNIQUE_ADDRESS with_empty e;
};

// Nested empty class objects should be able to share address,
// but on MSVC they don't.
#if !AC_COMPILER_MSVC()
static_assert(sizeof(with_nested_empty) == sizeof(int));
#endif
