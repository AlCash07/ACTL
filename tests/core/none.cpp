// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/core/none.hpp>

/* properties */
static_assert(std::is_empty_v<ac::none>);
static_assert(std::is_standard_layout_v<ac::none>);
static_assert(std::is_trivial_v<ac::none>);
/* is_none */
static_assert(ac::is_none_v<ac::none>);
static_assert(!ac::is_none_v<ac::none const>);
static_assert(!ac::is_none_v<void>);
/* void_to_none */
static_assert(std::is_same_v<int, ac::void_to_none_t<int>>);
static_assert(std::is_same_v<ac::none, ac::void_to_none_t<void>>);
static_assert(std::is_same_v<ac::none const, ac::void_to_none_t<void const>>);
