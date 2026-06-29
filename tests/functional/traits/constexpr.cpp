// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/traits/constexpr.hpp>

struct Array {
    static int size() {
        return 0;
    }
    static constexpr int constexpr_size() {
        return 0;
    }

    static void void_function() {}
    static constexpr void constexpr_void_function() {}
};

static_assert(!ac::is_constexpr([] { Array::size(); }));
static_assert(ac::is_constexpr([] { Array::constexpr_size(); }));
/* Testing void return separately,
   because it's problematic for some implementations. */
static_assert(!ac::is_constexpr([] { Array::void_function(); }));
static_assert(ac::is_constexpr([] { Array::constexpr_void_function(); }));

template<typename T = Array>
constexpr void test_AC_REQUIRES_CONSTEXPR() {
    static_assert(!AC_REQUIRES_CONSTEXPR(T::size()));
    static_assert(AC_REQUIRES_CONSTEXPR(T::constexpr_size()));
    static_assert(!AC_REQUIRES_CONSTEXPR(T::invalid_function()));
}
