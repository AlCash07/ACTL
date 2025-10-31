// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/qualifiers/copy.hpp>

/* copy_const_t */
/* unqualified target */
static_assert(std::is_same_v<int, ac::copy_const_t<int, char>>);
static_assert(std::is_same_v<
              int const,
              ac::copy_const_t<int, char const volatile>>);
static_assert(std::is_same_v<int, ac::copy_const_t<int, char const&>>);
static_assert(std::is_same_v<int, ac::copy_const_t<int, char volatile&&>>);
/* const target */
static_assert(std::is_same_v<int, ac::copy_const_t<int const, char>>);
static_assert(std::is_same_v<
              int const,
              ac::copy_const_t<int const, char const volatile>>);
static_assert(std::is_same_v<int, ac::copy_const_t<int const, char const&>>);
static_assert(std::
                  is_same_v<int, ac::copy_const_t<int const, char volatile&&>>);
/* reference target */
static_assert(std::is_same_v<int&, ac::copy_const_t<int&, char>>);
static_assert(std::
                  is_same_v<int&, ac::copy_const_t<int&, char const volatile>>);
static_assert(std::is_same_v<int&, ac::copy_const_t<int&, char const&>>);
static_assert(std::is_same_v<int&, ac::copy_const_t<int&, char volatile&&>>);

/* copy_volatile_t */
/* unqualified target */
static_assert(std::is_same_v<int, ac::copy_volatile_t<int, char>>);
static_assert(std::is_same_v<
              int volatile,
              ac::copy_volatile_t<int, char const volatile>>);
static_assert(std::is_same_v<int, ac::copy_volatile_t<int, char const&>>);
static_assert(std::is_same_v<int, ac::copy_volatile_t<int, char volatile&&>>);
/* const target */
static_assert(std::is_same_v<int const, ac::copy_volatile_t<int const, char>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::copy_volatile_t<int const, char const volatile>>);
static_assert(std::is_same_v<
              int const,
              ac::copy_volatile_t<int const, char const&>>);
static_assert(std::is_same_v<
              int const,
              ac::copy_volatile_t<int const, char volatile&&>>);
/* reference target */
static_assert(std::is_same_v<int&, ac::copy_volatile_t<int&, char>>);
static_assert(std::is_same_v<
              int&,
              ac::copy_volatile_t<int&, char const volatile>>);
static_assert(std::is_same_v<int&, ac::copy_volatile_t<int&, char const&>>);
static_assert(std::is_same_v<int&, ac::copy_volatile_t<int&, char volatile&&>>);

/* copy_cv_t */
/* unqualified target */
static_assert(std::is_same_v<int, ac::copy_cv_t<int, char>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::copy_cv_t<int, char const volatile>>);
static_assert(std::is_same_v<int, ac::copy_cv_t<int, char const&>>);
static_assert(std::is_same_v<int, ac::copy_cv_t<int, char volatile&&>>);
/* const target */
static_assert(std::is_same_v<int, ac::copy_cv_t<int const, char>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::copy_cv_t<int const, char const volatile>>);
static_assert(std::is_same_v<int, ac::copy_cv_t<int const, char const&>>);
static_assert(std::is_same_v<int, ac::copy_cv_t<int const, char volatile&&>>);
/* reference target */
static_assert(std::is_same_v<int&, ac::copy_cv_t<int&, char>>);
static_assert(std::is_same_v<int&, ac::copy_cv_t<int&, char const volatile>>);
static_assert(std::is_same_v<int&, ac::copy_cv_t<int&, char const&>>);
static_assert(std::is_same_v<int&, ac::copy_cv_t<int&, char volatile&&>>);

/* copy_reference_t */
/* unqualified target */
static_assert(std::is_same_v<int, ac::copy_reference_t<int, char>>);
static_assert(std::is_same_v<
              int,
              ac::copy_reference_t<int, char const volatile>>);
static_assert(std::is_same_v<int&, ac::copy_reference_t<int, char const&>>);
static_assert(std::
                  is_same_v<int&&, ac::copy_reference_t<int, char volatile&&>>);
/* const target */
static_assert(std::is_same_v<int const, ac::copy_reference_t<int const, char>>);
static_assert(std::is_same_v<
              int const,
              ac::copy_reference_t<int const, char const volatile>>);
static_assert(std::is_same_v<
              int const&,
              ac::copy_reference_t<int const, char const&>>);
static_assert(std::is_same_v<
              int const&&,
              ac::copy_reference_t<int const, char volatile&&>>);
/* reference target */
static_assert(std::is_same_v<int, ac::copy_reference_t<int&, char>>);
static_assert(std::is_same_v<
              int,
              ac::copy_reference_t<int&, char const volatile>>);
static_assert(std::is_same_v<int&, ac::copy_reference_t<int&, char const&>>);
static_assert(std::is_same_v<
              int&&,
              ac::copy_reference_t<int&, char volatile&&>>);

/* copy_cvref_t */
/* unqualified target */
static_assert(std::is_same_v<int, ac::copy_cvref_t<int, char>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::copy_cvref_t<int, char const volatile>>);
static_assert(std::is_same_v<int const&, ac::copy_cvref_t<int, char const&>>);
static_assert(std::is_same_v<
              int volatile&&,
              ac::copy_cvref_t<int, char volatile&&>>);
/* const target */
static_assert(std::is_same_v<int, ac::copy_cvref_t<int const, char>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::copy_cvref_t<int const, char const volatile>>);
static_assert(std::is_same_v<
              int const&,
              ac::copy_cvref_t<int const, char const&>>);
static_assert(std::is_same_v<
              int volatile&&,
              ac::copy_cvref_t<int const, char volatile&&>>);
/* reference target */
static_assert(std::is_same_v<int, ac::copy_cvref_t<int&, char>>);
static_assert(std::is_same_v<
              int const volatile,
              ac::copy_cvref_t<int&, char const volatile>>);
static_assert(std::is_same_v<int const&, ac::copy_cvref_t<int&, char const&>>);
static_assert(std::is_same_v<
              int volatile&&,
              ac::copy_cvref_t<int&, char volatile&&>>);
