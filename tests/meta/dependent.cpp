// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/dependent.hpp>
#include <actl/std/vector.hpp>

// Containers preserve const for value_type but pointers don't.
static_assert(std::is_same_v<int const, ac::value_type_t<int const[8]>>);

// Outer const affects reference for containers but not for pointers.
static_assert(std::is_same_v<int&, ac::reference_t<std::vector<int>>>);
static_assert(
    std::is_same_v<int const&, ac::reference_t<std::vector<int> const>>);

using VI = std::vector<int>;
static_assert(std::is_same_v<VI::iterator, ac::iterator_t<VI>>);
static_assert(std::is_same_v<VI::const_iterator, ac::iterator_t<VI const>>);
static_assert(std::is_same_v<int*, ac::iterator_t<int[8]>>);
static_assert(std::is_same_v<int const*, ac::iterator_t<int const[8]>>);
