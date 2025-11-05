// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/std/span.hpp>

static_assert(ac::Reference<std::span<int>>);
static_assert(ac::Reference<std::span<int const>>);
static_assert(ac::Reference<std::span<int, 2>>);
