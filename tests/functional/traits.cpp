// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/traits.hpp>
#include <functional>

using free_function_0_arity = void();
static_assert(0ul == ac::arity_v<free_function_0_arity>);
static_assert(std::is_same_v<void, ac::return_type_t<free_function_0_arity>>);

using free_function_ptr = free_function_0_arity*;
static_assert(0ul == ac::arity_v<free_function_ptr>);
static_assert(std::is_same_v<void, ac::return_type_t<free_function_ptr>>);

using free_function_params = int && (int, const int, int&, const int&, int*, const int*);
static_assert(6ul == ac::arity_v<free_function_params>);
static_assert(std::is_same_v<int&&, ac::return_type_t<free_function_params>>);
static_assert(std::is_same_v<int, ac::argument_type_t<0, free_function_params>>);
static_assert(std::is_same_v<int, ac::argument_type_t<1, free_function_params>>);
static_assert(std::is_same_v<int&, ac::argument_type_t<2, free_function_params>>);
static_assert(std::is_same_v<const int&, ac::argument_type_t<3, free_function_params>>);
static_assert(std::is_same_v<int*, ac::argument_type_t<4, free_function_params>>);
static_assert(std::is_same_v<const int*, ac::argument_type_t<5, free_function_params>>);

namespace {

struct S {
    const int& member_function(const int, int&&);

    int* const_member_function() const;
};

struct function_object {
    const void* operator()(const S*, int&&, S);
};

struct const_function_object {
    S operator()() const;
};

}  // namespace

using member_function = decltype(&S::member_function);
static_assert(3ul == ac::arity_v<member_function>);
static_assert(std::is_same_v<const int&, ac::return_type_t<member_function>>);
static_assert(std::is_same_v<S&, ac::argument_type_t<0, member_function>>);
static_assert(std::is_same_v<int, ac::argument_type_t<1, member_function>>);
static_assert(std::is_same_v<int&&, ac::argument_type_t<2, member_function>>);

using const_member_function = decltype(&S::const_member_function);
static_assert(1ul == ac::arity_v<const_member_function>);
static_assert(std::is_same_v<int*, ac::return_type_t<const_member_function>>);
static_assert(std::is_same_v<const S&, ac::argument_type_t<0, const_member_function>>);

static_assert(3ul == ac::arity_v<function_object>);
static_assert(3ul == ac::arity_v<function_object&>);
static_assert(3ul == ac::arity_v<function_object&&>);
static_assert(std::is_same_v<const void*, ac::return_type_t<function_object>>);
static_assert(std::is_same_v<const S*, ac::argument_type_t<0, function_object>>);
static_assert(std::is_same_v<int&&, ac::argument_type_t<1, function_object>>);
static_assert(std::is_same_v<S, ac::argument_type_t<2, function_object>>);

static_assert(0ul == ac::arity_v<const_function_object>);
static_assert(0ul == ac::arity_v<const const_function_object>);
static_assert(0ul == ac::arity_v<const const_function_object&>);
static_assert(std::is_same_v<S, ac::return_type_t<const const_function_object>>);

using std_function = std::function<int*(int&)>;
static_assert(1ul == ac::arity_v<std_function>);
static_assert(std::is_same_v<int*, ac::return_type_t<std_function>>);
static_assert(std::is_same_v<int&, ac::argument_type_t<0, std_function>>);
