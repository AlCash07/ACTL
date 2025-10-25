// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include "functional/traits/M.hpp"
#include "test.hpp"

template<auto Member, class ClassParameter, bool AcceptsVArgs, bool IsNoexcept>
void check() {
    using MF = decltype(Member);
    static_assert(std::is_same_v<int, ac::return_t<MF>>);
    static_assert(std::is_same_v<
                  ac::type_list<ClassParameter>,
                  ac::parameters_t<MF>>);
    static_assert(1ul == ac::arity_v<MF>);
    static_assert(std::is_same_v<ClassParameter, ac::class_parameter_t<MF>>);
    static_assert(std::is_same_v<M, ac::unqualified_class_t<MF>>);
    static_assert(AcceptsVArgs == ac::accepts_variadic_arguments_v<MF>);
    static_assert(IsNoexcept == ac::is_noexcept_v<MF>);
}

TEST_CASE("member function pointers") {
    check<&M::fn, M&, false, false>();
    check<&M::fn_c, M const&, false, false>();
    check<&M::fn_v, M volatile&, false, false>();
    check<&M::fn_cv, M const volatile&, false, false>();
    check<&M::fn_lref, M&, false, false>();
    check<&M::fn_c_lref, M const&, false, false>();
    check<&M::fn_v_lref, M volatile&, false, false>();
    check<&M::fn_cv_lref, M const volatile&, false, false>();
    check<&M::fn_rref, M&&, false, false>();
    check<&M::fn_c_rref, M const&&, false, false>();
    check<&M::fn_v_rref, M volatile&&, false, false>();
    check<&M::fn_cv_rref, M const volatile&&, false, false>();
    check<&M::fn_noexcept, M&, false, true>();
    check<&M::fn_c_noexcept, M const&, false, true>();
    check<&M::fn_v_noexcept, M volatile&, false, true>();
    check<&M::fn_cv_noexcept, M const volatile&, false, true>();
    check<&M::fn_lref_noexcept, M&, false, true>();
    check<&M::fn_c_lref_noexcept, M const&, false, true>();
    check<&M::fn_v_lref_noexcept, M volatile&, false, true>();
    check<&M::fn_cv_lref_noexcept, M const volatile&, false, true>();
    check<&M::fn_rref_noexcept, M&&, false, true>();
    check<&M::fn_c_rref_noexcept, M const&&, false, true>();
    check<&M::fn_v_rref_noexcept, M volatile&&, false, true>();
    check<&M::fn_cv_rref_noexcept, M const volatile&&, false, true>();
    /* variadic arguments */
    check<&M::fn_va, M&, true, false>();
    check<&M::fn_va_c, M const&, true, false>();
    check<&M::fn_va_v, M volatile&, true, false>();
    check<&M::fn_va_cv, M const volatile&, true, false>();
    check<&M::fn_va_lref, M&, true, false>();
    check<&M::fn_va_c_lref, M const&, true, false>();
    check<&M::fn_va_v_lref, M volatile&, true, false>();
    check<&M::fn_va_cv_lref, M const volatile&, true, false>();
    check<&M::fn_va_rref, M&&, true, false>();
    check<&M::fn_va_c_rref, M const&&, true, false>();
    check<&M::fn_va_v_rref, M volatile&&, true, false>();
    check<&M::fn_va_cv_rref, M const volatile&&, true, false>();
    check<&M::fn_va_noexcept, M&, true, true>();
    check<&M::fn_va_c_noexcept, M const&, true, true>();
    check<&M::fn_va_v_noexcept, M volatile&, true, true>();
    check<&M::fn_va_cv_noexcept, M const volatile&, true, true>();
    check<&M::fn_va_lref_noexcept, M&, true, true>();
    check<&M::fn_va_c_lref_noexcept, M const&, true, true>();
    check<&M::fn_va_v_lref_noexcept, M volatile&, true, true>();
    check<&M::fn_va_cv_lref_noexcept, M const volatile&, true, true>();
    check<&M::fn_va_rref_noexcept, M&&, true, true>();
    check<&M::fn_va_c_rref_noexcept, M const&&, true, true>();
    check<&M::fn_va_v_rref_noexcept, M volatile&&, true, true>();
    check<&M::fn_va_cv_rref_noexcept, M const volatile&&, true, true>();
    /* qualifying a member function pointer doesn't change its traits */
    check<static_cast<decltype(&M::fn) const&>(&M::fn), M&, false, false>();
    check<std::move(&M::fn_va_noexcept), M&, true, true>();
}

using fn_parameters = decltype(&M::fn_parameters);
static_assert(std::is_same_v<int const&, ac::return_t<fn_parameters>>);
static_assert(std::is_same_v<
              ac::type_list<M const&, int, int&&>,
              ac::parameters_t<fn_parameters>>);
static_assert(ac::accepts_variadic_arguments_v<fn_parameters>);
static_assert(ac::is_noexcept_v<fn_parameters>);
/* derived traits */
static_assert(3 == ac::arity_v<fn_parameters>);
static_assert(!ac::returns_void_v<fn_parameters>);
static_assert(std::is_same_v<M const&, ac::parameter_at_t<fn_parameters, 0>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<fn_parameters, 1>>);
static_assert(std::is_same_v<int&&, ac::parameter_at_t<fn_parameters, 2>>);

/* modification */

#define AC_MF_CHECK(expected, trait, input) \
    static_assert(std::is_same_v<decltype(expected), trait<decltype(input)>>);

/* variadic arguments modification */
/* add_variadic_arguments_t */
AC_MF_CHECK(&M::fn_va, ac::add_variadic_arguments_t, &M::fn_va); // unchanged
AC_MF_CHECK(&M::fn_va, ac::add_variadic_arguments_t, &M::fn);
AC_MF_CHECK(
    &M::fn_va_c_lref_noexcept,
    ac::add_variadic_arguments_t,
    &M::fn_c_lref_noexcept
);
/* remove_variadic_arguments_t */
AC_MF_CHECK(&M::fn, ac::remove_variadic_arguments_t, &M::fn); // unchanged
AC_MF_CHECK(&M::fn, ac::remove_variadic_arguments_t, &M::fn_va);
AC_MF_CHECK(
    &M::fn_c_rref_noexcept,
    ac::remove_variadic_arguments_t,
    &M::fn_va_c_rref_noexcept
);

/* noexcept modification */
/* add_noexcept_t */
AC_MF_CHECK(&M::fn_noexcept, ac::add_noexcept_t, &M::fn_noexcept); // unchanged
AC_MF_CHECK(&M::fn_noexcept, ac::add_noexcept_t, &M::fn);
AC_MF_CHECK(&M::fn_va_c_lref_noexcept, ac::add_noexcept_t, &M::fn_va_c_lref);
/* remove_noexcept_t */
AC_MF_CHECK(&M::fn, ac::remove_noexcept_t, &M::fn); // unchanged
AC_MF_CHECK(&M::fn, ac::remove_noexcept_t, &M::fn_noexcept);
AC_MF_CHECK(&M::fn_va_c_rref, ac::remove_noexcept_t, &M::fn_va_c_rref_noexcept);

#undef AC_MF_CHECK

/* as_member_of_t */
struct Dst {};
static_assert(std::is_same_v<int Dst::*, ac::as_member_of_t<int, Dst>>);
static_assert(std::is_same_v<
              void (Dst::*)(int),
              ac::as_member_of_t<void(int), Dst>>);
static_assert(std::is_same_v<
              void (Dst::*)(...) noexcept,
              ac::as_member_of_t<void(...) noexcept, Dst>>);
static_assert(std::is_same_v<
              int (Dst::*)(),
              ac::as_member_of_t<decltype(&M::fn), Dst>>);
static_assert(std::is_same_v<
              int (Dst::*)(...) const volatile & noexcept,
              ac::as_member_of_t<decltype(&M::fn_va_cv_lref_noexcept), Dst>>);
