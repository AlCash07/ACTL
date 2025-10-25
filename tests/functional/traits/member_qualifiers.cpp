// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/member_qualifiers.hpp>
#include "functional/traits/M.hpp"

/* has_member_qualifiers_v */
static_assert(!ac::has_member_qualifiers_v<decltype(&M::fn)>);
static_assert(!ac::has_member_qualifiers_v<decltype(&M::fn_va)>);
static_assert(!ac::has_member_qualifiers_v<decltype(&M::fn_noexcept)>);
static_assert(ac::has_member_qualifiers_v<decltype(&M::fn_c)>);
static_assert(ac::has_member_qualifiers_v<decltype(&M::fn_v)>);
static_assert(ac::has_member_qualifiers_v<decltype(&M::fn_lref)>);
static_assert(ac::has_member_qualifiers_v<decltype(&M::fn_rref)>);

#define AC_MF_CHECK(expected, trait, input) \
    static_assert(std::is_same_v<decltype(expected), trait<decltype(input)>>);

/* const */
static_assert(!ac::is_const_member_v<decltype(&M::fn)>);
static_assert(ac::is_const_member_v<decltype(&M::fn_c)>);
static_assert(!ac::is_const_member_v<decltype(&M::fn_va_lref_noexcept)>);
static_assert(ac::is_const_member_v<decltype(&M::fn_va_c_lref_noexcept)>);
AC_MF_CHECK(&M::fn_c, ac::add_member_const_t, &M::fn_c); // unchanged
AC_MF_CHECK(&M::fn_c, ac::add_member_const_t, &M::fn);
AC_MF_CHECK(
    &M::fn_va_c_lref_noexcept, ac::add_member_const_t, &M::fn_va_lref_noexcept
);
AC_MF_CHECK(&M::fn, ac::remove_member_const_t, &M::fn); // unchanged
AC_MF_CHECK(&M::fn, ac::remove_member_const_t, &M::fn_c);
AC_MF_CHECK(
    &M::fn_va_rref_noexcept,
    ac::remove_member_const_t,
    &M::fn_va_c_rref_noexcept
);

/* reference */
static_assert(!ac::is_reference_member_v<decltype(&M::fn)>);
static_assert(ac::is_reference_member_v<decltype(&M::fn_lref)>);
static_assert(ac::is_reference_member_v<decltype(&M::fn_rref)>);
static_assert(!ac::is_reference_member_v<decltype(&M::fn_va_c_noexcept)>);
static_assert(ac::is_reference_member_v<decltype(&M::fn_va_c_lref_noexcept)>);
static_assert(ac::is_reference_member_v<decltype(&M::fn_va_c_rref_noexcept)>);
AC_MF_CHECK(&M::fn, ac::remove_member_reference_t, &M::fn); // unchanged
AC_MF_CHECK(&M::fn, ac::remove_member_reference_t, &M::fn_lref);
AC_MF_CHECK(&M::fn, ac::remove_member_reference_t, &M::fn_rref);
AC_MF_CHECK(
    &M::fn_va_c_noexcept,
    ac::remove_member_reference_t,
    &M::fn_va_c_lref_noexcept
);
AC_MF_CHECK(
    &M::fn_va_c_noexcept,
    ac::remove_member_reference_t,
    &M::fn_va_c_rref_noexcept
);

/* lvalue reference */
static_assert(!ac::is_lvalue_reference_member_v<decltype(&M::fn)>);
static_assert(!ac::is_lvalue_reference_member_v<decltype(&M::fn_rref)>);
static_assert(ac::is_lvalue_reference_member_v<decltype(&M::fn_lref)>);
static_assert(!ac::is_lvalue_reference_member_v<decltype(&M::fn_va_c_noexcept
              )>);
static_assert(!ac::is_lvalue_reference_member_v<
              decltype(&M::fn_va_c_rref_noexcept)>);
static_assert(ac::is_lvalue_reference_member_v<
              decltype(&M::fn_va_c_lref_noexcept)>);
// unchanged
AC_MF_CHECK(&M::fn_lref, ac::add_member_lvalue_reference_t, &M::fn_lref);
AC_MF_CHECK(&M::fn_lref, ac::add_member_lvalue_reference_t, &M::fn_lref);
AC_MF_CHECK( // unchanged
    &M::fn_va_c_lref_noexcept,
    ac::add_member_lvalue_reference_t,
    &M::fn_va_c_lref_noexcept
);
AC_MF_CHECK(
    &M::fn_va_c_lref_noexcept,
    ac::add_member_lvalue_reference_t,
    &M::fn_va_c_noexcept
);
// lvalue reference prevails over rvalue reference
// according to the reference collapsing rules
AC_MF_CHECK(&M::fn_lref, ac::add_member_lvalue_reference_t, &M::fn_rref);
AC_MF_CHECK(
    &M::fn_va_c_lref_noexcept,
    ac::add_member_lvalue_reference_t,
    &M::fn_va_c_rref_noexcept
);

/* rvalue reference */
static_assert(!ac::is_rvalue_reference_member_v<decltype(&M::fn)>);
static_assert(!ac::is_rvalue_reference_member_v<decltype(&M::fn_lref)>);
static_assert(ac::is_rvalue_reference_member_v<decltype(&M::fn_rref)>);
static_assert(!ac::is_rvalue_reference_member_v<decltype(&M::fn_va_c_noexcept
              )>);
static_assert(!ac::is_rvalue_reference_member_v<
              decltype(&M::fn_va_c_lref_noexcept)>);
static_assert(ac::is_rvalue_reference_member_v<
              decltype(&M::fn_va_c_rref_noexcept)>);
// unchanged
AC_MF_CHECK(&M::fn_rref, ac::add_member_rvalue_reference_t, &M::fn_rref);
AC_MF_CHECK(&M::fn_rref, ac::add_member_rvalue_reference_t, &M::fn_rref);
AC_MF_CHECK( // unchanged
    &M::fn_va_c_rref_noexcept,
    ac::add_member_rvalue_reference_t,
    &M::fn_va_c_rref_noexcept
);
AC_MF_CHECK(
    &M::fn_va_c_rref_noexcept,
    ac::add_member_rvalue_reference_t,
    &M::fn_va_c_noexcept
);
// lvalue reference prevails over rvalue reference
// according to the reference collapsing rules
AC_MF_CHECK(&M::fn_lref, ac::add_member_rvalue_reference_t, &M::fn_lref);
AC_MF_CHECK(
    &M::fn_va_c_lref_noexcept,
    ac::add_member_rvalue_reference_t,
    &M::fn_va_c_lref_noexcept
);

/* volatile */
static_assert(!ac::is_volatile_member_v<decltype(&M::fn)>);
static_assert(ac::is_volatile_member_v<decltype(&M::fn_v)>);
static_assert(!ac::is_volatile_member_v<decltype(&M::fn_va_c_lref_noexcept)>);
static_assert(ac::is_volatile_member_v<decltype(&M::fn_va_cv_lref_noexcept)>);
AC_MF_CHECK(&M::fn_v, ac::add_member_volatile_t, &M::fn_v); // unchanged
AC_MF_CHECK(&M::fn_v, ac::add_member_volatile_t, &M::fn);
AC_MF_CHECK(
    &M::fn_va_cv_lref_noexcept,
    ac::add_member_volatile_t,
    &M::fn_va_c_lref_noexcept
);
AC_MF_CHECK(&M::fn, ac::remove_member_volatile_t, &M::fn); // unchanged
AC_MF_CHECK(&M::fn, ac::remove_member_volatile_t, &M::fn_v);
AC_MF_CHECK(
    &M::fn_va_c_rref_noexcept,
    ac::remove_member_volatile_t,
    &M::fn_va_cv_rref_noexcept
);

/* cv - const volatile */
static_assert(!ac::is_cv_member_v<decltype(&M::fn_c)>);
static_assert(!ac::is_cv_member_v<decltype(&M::fn_v)>);
static_assert(ac::is_cv_member_v<decltype(&M::fn_cv)>);
static_assert(!ac::is_cv_member_v<decltype(&M::fn_va_c_lref_noexcept)>);
static_assert(!ac::is_cv_member_v<decltype(&M::fn_va_v_lref_noexcept)>);
static_assert(ac::is_cv_member_v<decltype(&M::fn_va_cv_lref_noexcept)>);
AC_MF_CHECK(&M::fn_cv, ac::add_member_cv_t, &M::fn_cv); // unchanged
AC_MF_CHECK(&M::fn_cv, ac::add_member_cv_t, &M::fn);
AC_MF_CHECK(&M::fn_cv, ac::add_member_cv_t, &M::fn_c);
AC_MF_CHECK(&M::fn_cv, ac::add_member_cv_t, &M::fn_v);
AC_MF_CHECK(
    &M::fn_va_cv_lref_noexcept, ac::add_member_cv_t, &M::fn_va_lref_noexcept
);
AC_MF_CHECK(&M::fn, ac::remove_member_cv_t, &M::fn); // unchanged
AC_MF_CHECK(&M::fn, ac::remove_member_cv_t, &M::fn_c);
AC_MF_CHECK(&M::fn, ac::remove_member_cv_t, &M::fn_v);
AC_MF_CHECK(&M::fn, ac::remove_member_cv_t, &M::fn_cv);
AC_MF_CHECK(
    &M::fn_va_rref_noexcept, ac::remove_member_cv_t, &M::fn_va_cv_rref_noexcept
);

#undef AC_MF_CHECK
