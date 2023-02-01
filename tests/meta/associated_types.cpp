// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/associated_types.hpp>
#include <actl/range/iterator/traits/associated_types.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <set>
#include <vector>

// Associated types are working for a random access container.
static_assert(std::is_same_v<int, ac::value_t<std::vector<int>>>);
static_assert(std::is_same_v<int&, ac::reference_t<std::vector<int>>>);
// Adding const to a container affects reference_t but not value_t.
static_assert(std::is_same_v<int, ac::value_t<const std::vector<int>>>);
static_assert(
    std::is_same_v<int const&, ac::reference_t<const std::vector<int>>>);

// Associated types are working for a non-const associative container.
static_assert(std::is_same_v<int, ac::value_t<std::set<int>>>);
static_assert(std::is_same_v<int&, ac::reference_t<std::set<int>>>);
// Adding const to a container affects reference_t but not value_t.
static_assert(std::is_same_v<int, ac::value_t<const std::set<int>>>);
static_assert(std::is_same_v<int const&, ac::reference_t<const std::set<int>>>);
