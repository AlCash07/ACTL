// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/qualifiers/detection.hpp>

/* ac::is_cv_v */
static_assert(!ac::is_cv_v<void>);
static_assert(!ac::is_cv_v<int const>);
static_assert(!ac::is_cv_v<int volatile>);
static_assert(!ac::is_cv_v<int const volatile&>);
static_assert(ac::is_cv_v<int const volatile>);

/* ac::has_qualifiers_v */
static_assert(!ac::has_qualifiers_v<void>);
static_assert(ac::has_qualifiers_v<int const>);
static_assert(ac::has_qualifiers_v<int volatile>);
static_assert(ac::has_qualifiers_v<int&>);
static_assert(ac::has_qualifiers_v<int&&>);
static_assert(ac::has_qualifiers_v<int const volatile&>);
