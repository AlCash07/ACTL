// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/traits/nth_type.hpp>

static_assert(std::is_same_v<int, ac::nth_type_t<0, int>>);
static_assert(std::is_same_v<int, ac::nth_type_t<0, int, float>>);
static_assert(std::is_same_v<float, ac::nth_type_t<1, int, float>>);