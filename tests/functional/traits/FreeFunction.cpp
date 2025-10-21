// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include "test.hpp"

template<class Fn, bool AcceptsVArgs, bool IsNoexcept>
void test_void_free_function_traits() {
    static_assert(1ul == ac::arity_v<Fn>);
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

using free_function_params = //
    int && (int&, int const&, int*, int const*, ...) noexcept;
static_assert(4ul == ac::arity_v<free_function_params>);
static_assert(std::is_same_v<int&&, ac::return_t<free_function_params>>);
static_assert(std::is_same_v<
              ac::type_list<int&, int const&, int*, int const*>,
              ac::parameters_t<free_function_params>>);
static_assert(ac::accepts_variadic_arguments_v<free_function_params>);
static_assert(ac::is_noexcept_v<free_function_params>);
