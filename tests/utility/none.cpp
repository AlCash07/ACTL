// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/utility/none.hpp>
#include <type_traits>
#include "test.hpp"

static_assert(std::is_empty_v<ac::none>);

static_assert(std::is_same_v<int, ac::replace_void_t<int>>);
static_assert(std::is_same_v<ac::none, ac::replace_void_t<void>>);
static_assert(std::is_same_v<const ac::none, ac::replace_void_t<const void>>);
