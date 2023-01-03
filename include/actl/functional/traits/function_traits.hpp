// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/free_function_traits.hpp>
#include <actl/functional/traits/member_function_traits.hpp>

namespace ac {

/// Callable traits specialized only for functions (both free and member).
template <class T>
struct function_traits
{};

/// Concept of a function, including a member function.
template <class T>
concept Function = FreeFunction<T> || MemberFunction<T>;

template <FreeFunction Fn>
struct function_traits<Fn> : free_function_traits<Fn>
{};

template <MemberFunction Fn>
struct function_traits<Fn> : member_function_traits<Fn>
{};

} // namespace ac
