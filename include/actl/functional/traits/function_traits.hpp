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

/// Indicates whether a type is a function, including a member function,
/// or a function pointer (which in C++ is the same).
template <class T>
inline constexpr bool is_function_v =
    is_free_function_v<T> || is_member_function_v<T>;

template <class T>
requires is_free_function_v<T>
struct function_traits<T> : free_function_traits<T>
{};

template <class T>
requires is_member_function_v<T>
struct function_traits<T> : member_function_traits<T>
{};

} // namespace ac
