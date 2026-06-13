// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/sequence/tuple/at_tuple.hpp>
#include <tuple>

using namespace ac::constant_literals;

constexpr std::tuple t{'e', 4};
static_assert('e' == ac::at(t, 0_c));
static_assert(4 == ac::at(t, 1_c));
