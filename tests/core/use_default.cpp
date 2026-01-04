// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/core/use_default.hpp>

static_assert(std::is_same_v<float, ac::deduce_t<float, int>>);
static_assert(std::is_same_v<int, ac::deduce_t<ac::use_default, int>>);
