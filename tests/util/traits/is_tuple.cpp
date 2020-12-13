// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/std/utility.hpp>
#include <actl/test.hpp>
#include <actl/util/traits/is_tuple.hpp>

using namespace ac;

TEST("std::pair is a tuple") { static_assert(is_tuple_v<std::pair<int, int>>); }

TEST("std::tuple is a tuple") { static_assert(is_tuple_v<std::tuple<int, int, int>>); }

TEST("std::array is not a tuple") { static_assert(!is_tuple_v<std::array<int, 2>>); }