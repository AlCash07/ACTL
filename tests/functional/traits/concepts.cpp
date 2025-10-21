// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/traits/FreeFunction.hpp>
#include <actl/functional/traits/FunctionObject.hpp>
#include <actl/functional/traits/MemberFunction.hpp>

namespace {

struct Callable {
    bool operator()(int);

    bool member_function(int) const;
};

using free_function_t = bool(const Callable&, int);
static_assert(ac::FreeFunction<free_function_t>);
static_assert(!ac::MemberFunction<free_function_t>);
static_assert(!ac::FunctionObject<free_function_t>);

using member_function_t = decltype(&Callable::member_function);
static_assert(!ac::FreeFunction<member_function_t>);
static_assert(ac::MemberFunction<member_function_t>);
static_assert(!ac::FunctionObject<member_function_t>);

static_assert(!ac::FreeFunction<Callable>);
static_assert(!ac::MemberFunction<Callable>);
static_assert(ac::FunctionObject<Callable>);
// TODO: make this test pass
// static_assert(!ac::FunctionObject<const Callable>);

} // namespace
