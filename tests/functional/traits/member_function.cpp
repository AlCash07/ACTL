// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include "test.hpp"

namespace {

struct S {
    // The following abbreviations are used to make the test code concise,
    // because they are hopefully obvious from the member function declaration:
    // - c = const
    // - v = volatile
    // - cv = const volatile
    // - ref = reference
    // - rref = rvalue reference
    // - va = variadic arguments
    int fn();
    int fn_c() const;
    int fn_v() volatile;
    int fn_cv() const volatile;

    int fn_ref() &;
    int fn_c_ref() const&;
    int fn_v_ref() volatile&;
    int fn_cv_ref() const volatile&;

    int fn_rref() &&;
    int fn_c_rref() const&&;
    int fn_v_rref() volatile&&;
    int fn_cv_rref() const volatile&&;

    int fn_noexcept() noexcept;
    int fn_c_noexcept() const noexcept;
    int fn_v_noexcept() volatile noexcept;
    int fn_cv_noexcept() const volatile noexcept;

    int fn_ref_noexcept() & noexcept;
    int fn_c_ref_noexcept() const& noexcept;
    int fn_v_ref_noexcept() volatile& noexcept;
    int fn_cv_ref_noexcept() const volatile& noexcept;

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

    int fn_va_ref(...) &;
    int fn_va_c_ref(...) const&;
    int fn_va_v_ref(...) volatile&;
    int fn_va_cv_ref(...) const volatile&;

    int fn_va_rref(...) &&;
    int fn_va_c_rref(...) const&&;
    int fn_va_v_rref(...) volatile&&;
    int fn_va_cv_rref(...) const volatile&&;

    int fn_va_noexcept(...) noexcept;
    int fn_va_c_noexcept(...) const noexcept;
    int fn_va_v_noexcept(...) volatile noexcept;
    int fn_va_cv_noexcept(...) const volatile noexcept;

    int fn_va_ref_noexcept(...) & noexcept;
    int fn_va_c_ref_noexcept(...) const& noexcept;
    int fn_va_v_ref_noexcept(...) volatile& noexcept;
    int fn_va_cv_ref_noexcept(...) const volatile& noexcept;

    int fn_va_rref_noexcept(...) && noexcept;
    int fn_va_c_rref_noexcept(...) const&& noexcept;
    int fn_va_v_rref_noexcept(...) volatile&& noexcept;
    int fn_va_cv_rref_noexcept(...) const volatile&& noexcept;

    int const& fn_params(int const, int&&) const noexcept;
};

template<
    class MemberFn,
    class ClassParameter,
    bool AcceptsVarargs,
    bool IsNoexcept>
void check() {
    static_assert(1ul == ac::arity_v<MemberFn>);
    static_assert(std::is_same_v<int, ac::return_t<MemberFn>>);
    static_assert(std::is_same_v<
                  ClassParameter,
                  ac::parameter_at_t<0, MemberFn>>);
    static_assert(AcceptsVarargs == ac::accepts_variadic_arguments_v<MemberFn>);
    static_assert(IsNoexcept == ac::is_noexcept_v<MemberFn>);
}

} // namespace

TEST_CASE("member function pointers") {
    check<decltype(&S::fn), S&, false, false>();
    check<decltype(&S::fn_c), S const&, false, false>();
    check<decltype(&S::fn_v), S volatile&, false, false>();
    check<decltype(&S::fn_cv), S const volatile&, false, false>();
    check<decltype(&S::fn_ref), S&, false, false>();
    check<decltype(&S::fn_c_ref), S const&, false, false>();
    check<decltype(&S::fn_v_ref), S volatile&, false, false>();
    check<decltype(&S::fn_cv_ref), S const volatile&, false, false>();
    check<decltype(&S::fn_rref), S&&, false, false>();
    check<decltype(&S::fn_c_rref), S const&&, false, false>();
    check<decltype(&S::fn_v_rref), S volatile&&, false, false>();
    check<decltype(&S::fn_cv_rref), S const volatile&&, false, false>();
    check<decltype(&S::fn_noexcept), S&, false, true>();
    check<decltype(&S::fn_c_noexcept), S const&, false, true>();
    check<decltype(&S::fn_v_noexcept), S volatile&, false, true>();
    check<decltype(&S::fn_cv_noexcept), S const volatile&, false, true>();
    check<decltype(&S::fn_ref_noexcept), S&, false, true>();
    check<decltype(&S::fn_c_ref_noexcept), S const&, false, true>();
    check<decltype(&S::fn_v_ref_noexcept), S volatile&, false, true>();
    check<decltype(&S::fn_cv_ref_noexcept), S const volatile&, false, true>();
    check<decltype(&S::fn_rref_noexcept), S&&, false, true>();
    check<decltype(&S::fn_c_rref_noexcept), S const&&, false, true>();
    check<decltype(&S::fn_v_rref_noexcept), S volatile&&, false, true>();
    check<decltype(&S::fn_cv_rref_noexcept), S const volatile&&, false, true>();
    /* variadic arguments */
    check<decltype(&S::fn_va), S&, true, false>();
    check<decltype(&S::fn_va_c), S const&, true, false>();
    check<decltype(&S::fn_va_v), S volatile&, true, false>();
    check<decltype(&S::fn_va_cv), S const volatile&, true, false>();
    check<decltype(&S::fn_va_ref), S&, true, false>();
    check<decltype(&S::fn_va_c_ref), S const&, true, false>();
    check<decltype(&S::fn_va_v_ref), S volatile&, true, false>();
    check<decltype(&S::fn_va_cv_ref), S const volatile&, true, false>();
    check<decltype(&S::fn_va_rref), S&&, true, false>();
    check<decltype(&S::fn_va_c_rref), S const&&, true, false>();
    check<decltype(&S::fn_va_v_rref), S volatile&&, true, false>();
    check<decltype(&S::fn_va_cv_rref), S const volatile&&, true, false>();
    check<decltype(&S::fn_va_noexcept), S&, true, true>();
    check<decltype(&S::fn_va_c_noexcept), S const&, true, true>();
    check<decltype(&S::fn_va_v_noexcept), S volatile&, true, true>();
    check<decltype(&S::fn_va_cv_noexcept), S const volatile&, true, true>();
    check<decltype(&S::fn_va_ref_noexcept), S&, true, true>();
    check<decltype(&S::fn_va_c_ref_noexcept), S const&, true, true>();
    check<decltype(&S::fn_va_v_ref_noexcept), S volatile&, true, true>();
    check<decltype(&S::fn_va_cv_ref_noexcept), S const volatile&, true, true>();
    check<decltype(&S::fn_va_rref_noexcept), S&&, true, true>();
    check<decltype(&S::fn_va_c_rref_noexcept), S const&&, true, true>();
    check<decltype(&S::fn_va_v_rref_noexcept), S volatile&&, true, true>();
    check<decltype(&S::fn_va_cv_rref_noexcept), S const volatile&&, true, true>(
    );
    /* qualifying a member function pointer doesn't change its traits */
    check<decltype(&S::fn_noexcept) const&, S&, false, true>();
    check<decltype(&S::fn_noexcept)&&, S&, false, true>();
}

using fn_params = decltype(&S::fn_params);
static_assert(3ul == ac::arity_v<fn_params>);
static_assert(std::is_same_v<int const&, ac::return_t<fn_params>>);
static_assert(std::is_same_v<S const&, ac::parameter_at_t<0, fn_params>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<1, fn_params>>);
static_assert(std::is_same_v<int&&, ac::parameter_at_t<2, fn_params>>);
static_assert(ac::is_noexcept_v<fn_params>);
