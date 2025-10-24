// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/qualifiers/inner.hpp>

/* add_inner_const_t */
static_assert(std::is_same_v<int const, ac::add_inner_const_t<int>>);
static_assert(std::is_same_v<int const, ac::add_inner_const_t<int const>>);
static_assert(std::is_same_v<int const&, ac::add_inner_const_t<int&>>);
static_assert(std::is_same_v<int const&, ac::add_inner_const_t<int const&>>);
static_assert(std::is_same_v<int const&&, ac::add_inner_const_t<int&&>>);
/* remove_inner_const_t */
static_assert(std::is_same_v<int, ac::remove_inner_const_t<int const>>);
static_assert(std::is_same_v<int, ac::remove_inner_const_t<int>>);
static_assert(std::is_same_v<int&, ac::remove_inner_const_t<int const&>>);
static_assert(std::is_same_v<int&, ac::remove_inner_const_t<int&>>);
static_assert(std::is_same_v<int&&, ac::remove_inner_const_t<int const&&>>);

/* add_inner_volatile_t */
static_assert(std::is_same_v<int volatile, ac::add_inner_volatile_t<int>>);
static_assert(std::is_same_v<
              int volatile,
              ac::add_inner_volatile_t<int volatile>>);
static_assert(std::is_same_v<int volatile&, ac::add_inner_volatile_t<int&>>);
static_assert(std::is_same_v<
              int volatile&,
              ac::add_inner_volatile_t<int volatile&>>);
static_assert(std::is_same_v<int volatile&&, ac::add_inner_volatile_t<int&&>>);
/* remove_inner_volatile_t */
static_assert(std::is_same_v<int, ac::remove_inner_volatile_t<int volatile>>);
static_assert(std::is_same_v<int, ac::remove_inner_volatile_t<int>>);
static_assert(std::is_same_v<int&, ac::remove_inner_volatile_t<int volatile&>>);
static_assert(std::is_same_v<int&, ac::remove_inner_volatile_t<int&>>);
static_assert(std::is_same_v<
              int&&,
              ac::remove_inner_volatile_t<int volatile&&>>);

/* add_inner_cv_t */
static_assert(std::is_same_v<int const volatile, ac::add_inner_cv_t<int>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::add_inner_cv_t<int const volatile>>);
static_assert(std::is_same_v<int const volatile&, ac::add_inner_cv_t<int&>>);
static_assert(std::is_same_v<
              int const volatile&,
              ac::add_inner_cv_t<int const volatile&>>);
static_assert(std::is_same_v<int const volatile&&, ac::add_inner_cv_t<int&&>>);
/* remove_inner_cv_t */
static_assert(std::is_same_v<int, ac::remove_inner_cv_t<int const volatile>>);
static_assert(std::is_same_v<int, ac::remove_inner_cv_t<int>>);
static_assert(std::is_same_v<int&, ac::remove_inner_cv_t<int const volatile&>>);
static_assert(std::is_same_v<int&, ac::remove_inner_cv_t<int&>>);
static_assert(std::is_same_v<
              int&&,
              ac::remove_inner_cv_t<int const volatile&&>>);
