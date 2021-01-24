// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/is_template_base_of.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

namespace {
class public_derived : public std::vector<int> {};
class private_derived : private std::vector<int> {};
} // namespace

static_assert(ac::is_template_base_of_v<std::vector, std::vector<double>>);
static_assert(ac::is_template_base_of_v<std::vector, public_derived>);
static_assert(ac::is_template_base_of_v<std::vector, private_derived>);
static_assert(!ac::is_template_base_of_v<std::vector, std::pair<int, int>>);
static_assert(!ac::is_template_base_of_v<std::vector, int>);
