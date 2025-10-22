// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include "test.hpp"

// Struct with all the possible qualifier combinations for member functions.
struct S {
    // The following abbreviations are used to make the test code concise,
    // because they are hopefully obvious from the member function declaration:
    // - c = const
    // - v = volatile
    // - cv = const volatile
    // - lref = lvalue reference
    // - rref = rvalue reference
    // - va = variadic arguments
    int fn();
    int fn_c() const;
    int fn_v() volatile;
    int fn_cv() const volatile;

    int fn_lref() &;
    int fn_c_lref() const&;
    int fn_v_lref() volatile&;
    int fn_cv_lref() const volatile&;

    int fn_rref() &&;
    int fn_c_rref() const&&;
    int fn_v_rref() volatile&&;
    int fn_cv_rref() const volatile&&;

    int fn_noexcept() noexcept;
    int fn_c_noexcept() const noexcept;
    int fn_v_noexcept() volatile noexcept;
    int fn_cv_noexcept() const volatile noexcept;

    int fn_lref_noexcept() & noexcept;
    int fn_c_lref_noexcept() const& noexcept;
    int fn_v_lref_noexcept() volatile& noexcept;
    int fn_cv_lref_noexcept() const volatile& noexcept;

    int fn_rref_noexcept() && noexcept;
    int fn_c_rref_noexcept() const&& noexcept;
    int fn_v_rref_noexcept() volatile&& noexcept;
    int fn_cv_rref_noexcept() const volatile&& noexcept;

    // Variadic arguments are tested last,
    // because they are less common in code.
    int fn_va(...);
    int fn_va_c(...) const;
    int fn_va_v(...) volatile;
    int fn_va_cv(...) const volatile;

    int fn_va_lref(...) &;
    int fn_va_c_lref(...) const&;
    int fn_va_v_lref(...) volatile&;
    int fn_va_cv_lref(...) const volatile&;

    int fn_va_rref(...) &&;
    int fn_va_c_rref(...) const&&;
    int fn_va_v_rref(...) volatile&&;
    int fn_va_cv_rref(...) const volatile&&;

    int fn_va_noexcept(...) noexcept;
    int fn_va_c_noexcept(...) const noexcept;
    int fn_va_v_noexcept(...) volatile noexcept;
    int fn_va_cv_noexcept(...) const volatile noexcept;

    int fn_va_lref_noexcept(...) & noexcept;
    int fn_va_c_lref_noexcept(...) const& noexcept;
    int fn_va_v_lref_noexcept(...) volatile& noexcept;
    int fn_va_cv_lref_noexcept(...) const volatile& noexcept;

    int fn_va_rref_noexcept(...) && noexcept;
    int fn_va_c_rref_noexcept(...) const&& noexcept;
    int fn_va_v_rref_noexcept(...) volatile&& noexcept;
    int fn_va_cv_rref_noexcept(...) const volatile&& noexcept;

    int const& fn_params(int const, int&&, ...) const noexcept;
};

template<auto Member, class ClassParameter, bool AcceptsVArgs, bool IsNoexcept>
void check() {
    using MemberFn = decltype(Member);
    static_assert(1ul == ac::arity_v<MemberFn>);
    static_assert(std::is_same_v<int, ac::return_t<MemberFn>>);
    static_assert(std::is_same_v<
                  ClassParameter,
                  ac::parameter_at_t<0, MemberFn>>);
    static_assert(AcceptsVArgs == ac::accepts_variadic_arguments_v<MemberFn>);
    static_assert(IsNoexcept == ac::is_noexcept_v<MemberFn>);
}

TEST_CASE("member function pointers") {
    check<&S::fn, S&, false, false>();
    check<&S::fn_c, S const&, false, false>();
    check<&S::fn_v, S volatile&, false, false>();
    check<&S::fn_cv, S const volatile&, false, false>();
    check<&S::fn_lref, S&, false, false>();
    check<&S::fn_c_lref, S const&, false, false>();
    check<&S::fn_v_lref, S volatile&, false, false>();
    check<&S::fn_cv_lref, S const volatile&, false, false>();
    check<&S::fn_rref, S&&, false, false>();
    check<&S::fn_c_rref, S const&&, false, false>();
    check<&S::fn_v_rref, S volatile&&, false, false>();
    check<&S::fn_cv_rref, S const volatile&&, false, false>();
    check<&S::fn_noexcept, S&, false, true>();
    check<&S::fn_c_noexcept, S const&, false, true>();
    check<&S::fn_v_noexcept, S volatile&, false, true>();
    check<&S::fn_cv_noexcept, S const volatile&, false, true>();
    check<&S::fn_lref_noexcept, S&, false, true>();
    check<&S::fn_c_lref_noexcept, S const&, false, true>();
    check<&S::fn_v_lref_noexcept, S volatile&, false, true>();
    check<&S::fn_cv_lref_noexcept, S const volatile&, false, true>();
    check<&S::fn_rref_noexcept, S&&, false, true>();
    check<&S::fn_c_rref_noexcept, S const&&, false, true>();
    check<&S::fn_v_rref_noexcept, S volatile&&, false, true>();
    check<&S::fn_cv_rref_noexcept, S const volatile&&, false, true>();
    /* variadic arguments */
    check<&S::fn_va, S&, true, false>();
    check<&S::fn_va_c, S const&, true, false>();
    check<&S::fn_va_v, S volatile&, true, false>();
    check<&S::fn_va_cv, S const volatile&, true, false>();
    check<&S::fn_va_lref, S&, true, false>();
    check<&S::fn_va_c_lref, S const&, true, false>();
    check<&S::fn_va_v_lref, S volatile&, true, false>();
    check<&S::fn_va_cv_lref, S const volatile&, true, false>();
    check<&S::fn_va_rref, S&&, true, false>();
    check<&S::fn_va_c_rref, S const&&, true, false>();
    check<&S::fn_va_v_rref, S volatile&&, true, false>();
    check<&S::fn_va_cv_rref, S const volatile&&, true, false>();
    check<&S::fn_va_noexcept, S&, true, true>();
    check<&S::fn_va_c_noexcept, S const&, true, true>();
    check<&S::fn_va_v_noexcept, S volatile&, true, true>();
    check<&S::fn_va_cv_noexcept, S const volatile&, true, true>();
    check<&S::fn_va_lref_noexcept, S&, true, true>();
    check<&S::fn_va_c_lref_noexcept, S const&, true, true>();
    check<&S::fn_va_v_lref_noexcept, S volatile&, true, true>();
    check<&S::fn_va_cv_lref_noexcept, S const volatile&, true, true>();
    check<&S::fn_va_rref_noexcept, S&&, true, true>();
    check<&S::fn_va_c_rref_noexcept, S const&&, true, true>();
    check<&S::fn_va_v_rref_noexcept, S volatile&&, true, true>();
    check<&S::fn_va_cv_rref_noexcept, S const volatile&&, true, true>();
    /* qualifying a member function pointer doesn't change its traits */
    check<static_cast<decltype(&S::fn) const&>(&S::fn), S&, false, false>();
    check<std::move(&S::fn_va_noexcept), S&, true, true>();
}

using fn_params = decltype(&S::fn_params);
static_assert(3ul == ac::arity_v<fn_params>);
static_assert(std::is_same_v<int const&, ac::return_t<fn_params>>);
static_assert(std::is_same_v<
              ac::type_list<S const&, int, int&&>,
              ac::parameters_t<fn_params>>);
static_assert(ac::accepts_variadic_arguments_v<fn_params>);
static_assert(ac::is_noexcept_v<fn_params>);

#define AC_MF_CHECK(expected, trait, input) \
    static_assert(std::is_same_v<decltype(expected), trait<decltype(input)>>);

/* variadic arguments modification */
// add_variadic_arguments_t
AC_MF_CHECK(&S::fn_va, ac::add_variadic_arguments_t, &S::fn);
AC_MF_CHECK(&S::fn_va, ac::add_variadic_arguments_t, &S::fn_va); // unchanged
AC_MF_CHECK(
    &S::fn_va_c_lref_noexcept,
    ac::add_variadic_arguments_t,
    &S::fn_c_lref_noexcept
);
// remove_variadic_arguments_t
AC_MF_CHECK(&S::fn, ac::remove_variadic_arguments_t, &S::fn_va);
AC_MF_CHECK(&S::fn, ac::remove_variadic_arguments_t, &S::fn); // unchanged
AC_MF_CHECK(
    &S::fn_c_rref_noexcept,
    ac::remove_variadic_arguments_t,
    &S::fn_va_c_rref_noexcept
);

/* noexcept modification */
// add_noexcept_t
AC_MF_CHECK(&S::fn_noexcept, ac::add_noexcept_t, &S::fn);
AC_MF_CHECK(&S::fn_noexcept, ac::add_noexcept_t, &S::fn_noexcept); // unchanged
AC_MF_CHECK(&S::fn_va_v_lref_noexcept, ac::add_noexcept_t, &S::fn_va_v_lref);
// remove_noexcept_t
AC_MF_CHECK(&S::fn, ac::remove_noexcept_t, &S::fn_noexcept);
AC_MF_CHECK(&S::fn, ac::remove_noexcept_t, &S::fn); // unchanged
AC_MF_CHECK(&S::fn_va_c_rref, ac::remove_noexcept_t, &S::fn_va_c_rref_noexcept);

#undef AC_MF_CHECK
