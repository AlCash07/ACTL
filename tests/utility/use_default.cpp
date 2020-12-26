// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/utility/use_default.hpp>

static_assert(std::is_same_v<double, ac::deduce_t<double, int>>);
static_assert(std::is_same_v<int, ac::deduce_t<use_default, int>>);
