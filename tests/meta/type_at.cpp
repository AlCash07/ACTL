// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/type_at.hpp>
#include "test.hpp"

static_assert(std::is_same_v<int, ac::type_at_t<0, int>>);
static_assert(std::is_same_v<int, ac::type_at_t<0, int, float>>);
static_assert(std::is_same_v<float, ac::type_at_t<1, int, float>>);
