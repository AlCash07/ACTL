// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

// Struct with all the possible qualifier combinations for member functions.
struct M {
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

    int const& fn_parameters(int const, int&&, ...) const noexcept;
};
