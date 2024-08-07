// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/traits.hpp>
#include <actl/std/vector.hpp>
#include <memory>
#include "test.hpp"

static_assert(std::is_same_v<
              ac::dummy_container,
              ac::rebind_container_t<none, int>>);
static_assert(std::is_same_v<
              ac::dummy_container,
              ac::rebind_container_t<ac::dummy_container, int>>);
static_assert(std::is_same_v<
              ac::dummy_container,
              ac::rebind_container_t<std::vector<int>, none>>);
static_assert(std::is_same_v<
              std::vector<int>,
              ac::rebind_container_t<std::vector<none>, int>>);
