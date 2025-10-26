// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include "test.hpp"

template<class Fn, bool AcceptsVArgs, bool IsNoexcept>
void test_void_free_function_traits() {
    static_assert(std::is_same_v<void, ac::return_t<Fn>>);
    static_assert(std::is_same_v<ac::type_list<int>, ac::parameters_t<Fn>>);
    static_assert(AcceptsVArgs == ac::accepts_variadic_arguments_v<Fn>);
    static_assert(IsNoexcept == ac::is_noexcept_v<Fn>);
}

// One parameter is added to make the test stronger.
using fn = void(int);
using fn_noexcept = void(int) noexcept;
using fn_va = void(int, ...);
using fn_va_noexcept = void(int, ...) noexcept;

TEST_CASE("free function traits") {
    test_void_free_function_traits<fn, false, false>();
    test_void_free_function_traits<fn_noexcept, false, true>();
    test_void_free_function_traits<fn_va, true, false>();
    test_void_free_function_traits<fn_va_noexcept, true, true>();
    // const qualifier is ignored in a function parameter
    test_void_free_function_traits<void(const int), false, false>();
    // qualifiers don't matter for a free function
    test_void_free_function_traits<fn*, false, false>();
    test_void_free_function_traits<fn_noexcept&, false, true>();
    test_void_free_function_traits<fn_va&&, true, false>();
    test_void_free_function_traits<fn_va_noexcept* const&, true, true>();
}

using fn_parameters = //
    int && (int&, int const&, int*, int const*, ...) noexcept;
static_assert(std::is_same_v<int&&, ac::return_t<fn_parameters>>);
static_assert(std::is_same_v<
              ac::type_list<int&, int const&, int*, int const*>,
              ac::parameters_t<fn_parameters>>);
static_assert(ac::accepts_variadic_arguments_v<fn_parameters>);
static_assert(ac::is_noexcept_v<fn_parameters>);

/* derived traits */
/* returns_void_v */
static_assert(ac::returns_void_v<fn>);
static_assert(!ac::returns_void_v<fn_parameters>);
/* arity_v */
static_assert(1ul == ac::arity_v<fn>);
static_assert(4ul == ac::arity_v<fn_parameters>);
/* parameter_at_t */
static_assert(std::is_same_v<int&, ac::parameter_at_t<fn_parameters, 0>>);
static_assert(std::is_same_v<int const&, ac::parameter_at_t<fn_parameters, 1>>);
static_assert(std::is_same_v<int*, ac::parameter_at_t<fn_parameters, 2>>);
static_assert(std::is_same_v<int const*, ac::parameter_at_t<fn_parameters, 3>>);

/* variadic arguments modification */
/* add_variadic_arguments_t */
static_assert(std::is_same_v<fn_va, ac::add_variadic_arguments_t<fn>>);
static_assert(std::is_same_v<
              fn_va_noexcept,
              ac::add_variadic_arguments_t<fn_noexcept>>);
// unchanged
static_assert(std::is_same_v<fn_va, ac::add_variadic_arguments_t<fn_va>>);
/* remove_variadic_arguments_t */
static_assert(std::is_same_v<fn, ac::remove_variadic_arguments_t<fn_va>>);
static_assert(std::is_same_v<
              fn_noexcept,
              ac::remove_variadic_arguments_t<fn_va_noexcept>>);
// unchanged
static_assert(std::is_same_v<fn, ac::remove_variadic_arguments_t<fn>>);

/* noexcept modification */
/* add_noexcept_t */
static_assert(std::is_same_v<fn_noexcept, ac::add_noexcept_t<fn>>);
static_assert(std::is_same_v<fn_va_noexcept, ac::add_noexcept_t<fn_va>>);
// unchanged
static_assert(std::is_same_v<fn_noexcept, ac::add_noexcept_t<fn_noexcept>>);
/* remove_noexcept_t */
static_assert(std::is_same_v<fn, ac::remove_noexcept_t<fn_noexcept>>);
static_assert(std::is_same_v<fn_va, ac::remove_noexcept_t<fn_va_noexcept>>);
static_assert(std::is_same_v<fn, ac::remove_noexcept_t<fn>>); // unchanged

/* with_return_type_t */
static_assert(std::is_same_v<
              char(int, long),
              ac::with_return_type_t<ac::type_list<int, long>, char>>);
static_assert(std::is_same_v<char(int), ac::with_return_type_t<fn, char>>);
static_assert(std::is_same_v<
              char(int, ...) noexcept,
              ac::with_return_type_t<fn_va_noexcept, char>>);

/* as_free_function_t */
static_assert(std::is_same_v<fn, ac::as_free_function_t<fn>>);
static_assert(std::is_same_v<
              fn_va_noexcept,
              ac::as_free_function_t<fn_va_noexcept>>);
