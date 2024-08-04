// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include "test.hpp"

namespace {

template<class Fn, bool IsNoexcept>
void test_void_free_function_traits() {
    static_assert(0ul == ac::arity_v<Fn>);
    static_assert(std::is_same_v<void, ac::return_t<Fn>>);
    static_assert(IsNoexcept == ac::is_noexcept_v<Fn>);
}

} // namespace

TEST_CASE("void 0 arity free function: value, pointer, references") {
    test_void_free_function_traits<void(), false>();
    test_void_free_function_traits<void() noexcept, true>();
    test_void_free_function_traits<void (*)(), false>();
    test_void_free_function_traits<void (*)() noexcept, true>();
    test_void_free_function_traits<void (&)(), false>();
    test_void_free_function_traits<void (&)() noexcept, true>();
    test_void_free_function_traits<void (*const&)(), false>();
    test_void_free_function_traits<void (*const&)() noexcept, true>();
    test_void_free_function_traits<void (&&)(), false>();
    test_void_free_function_traits<void (&&)() noexcept, true>();
}

using free_function_params =
    int && (int, int const, int&, int const&, int*, int const*) noexcept;
static_assert(6ul == ac::arity_v<free_function_params>);
static_assert(std::is_same_v<int&&, ac::return_t<free_function_params>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<0, free_function_params>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<1, free_function_params>>);
static_assert(std::
                  is_same_v<int&, ac::parameter_at_t<2, free_function_params>>);
static_assert(std::is_same_v<
              int const&,
              ac::parameter_at_t<3, free_function_params>>);
static_assert(std::
                  is_same_v<int*, ac::parameter_at_t<4, free_function_params>>);
static_assert(std::is_same_v<
              int const*,
              ac::parameter_at_t<5, free_function_params>>);
static_assert(ac::is_noexcept_v<free_function_params>);
