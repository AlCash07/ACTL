// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/tuple.hpp>
#include <actl/std/array.hpp>
#include <actl/std/tuple.hpp>
#include <actl/std/utility.hpp>

static_assert(ac::Tuple<std::pair<int, int>>);
static_assert(ac::Tuple<std::tuple<int, int, int>>);
static_assert(ac::Tuple<std::array<int, 2>>);
