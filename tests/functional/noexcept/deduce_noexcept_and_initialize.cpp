// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/noexcept/deduce_noexcept_and_initialize.hpp>
#include "functional/noexcept/is_int.hpp"

struct Base {
    bool value;

    template <class T>
    constexpr Base(T x) noexcept(noexcept(ac::is_int(x)))
        : value{ac::is_int(x)} {}
};

struct Derived : Base {
    template <class T>
    constexpr Derived(T x) AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(Base{x}) {}
};

static_assert(noexcept(Derived{0}));
static_assert(Derived{0}.value);
static_assert(!noexcept(Derived{-1.0}));
static_assert(!Derived{0.0}.value);
