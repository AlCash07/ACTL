// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/iterator/traits/dependent.hpp>

static_assert(std::is_same_v<int, ac::iter_value_t<int const* const>>);

// Outer const affects reference for containers but not for pointers.
static_assert(std::is_same_v<int&, ac::iter_reference_t<int* const>>);
static_assert(std::is_same_v<int const&, ac::iter_reference_t<int const*>>);
