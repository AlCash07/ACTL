// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/function_object_traits.hpp>
#include <actl/functional/traits/function_traits.hpp>
#include <type_traits>

namespace ac {

template <class Fn>
struct callable_traits
{};

// Type qualifiers don't matter for regular functions.
template <class Fn>
requires is_function_v<std::remove_cvref_t<Fn>>
struct callable_traits<Fn> : function_traits<std::remove_cvref_t<Fn>>
{};

template <class Fn>
requires is_function_object_v<std::remove_reference_t<Fn>>
struct callable_traits<Fn> : function_object_traits<std::remove_reference_t<Fn>>
{};

/* convenience aliases */

template <class Fn>
using return_type_t = typename callable_traits<Fn>::return_type;

template <class Fn>
inline constexpr size_t arity_v = callable_traits<Fn>::arity;

template <size_t Index, class Fn>
using parameter_type_t =
    typename callable_traits<Fn>::template parameter_type<Index>;

template <class Fn>
inline constexpr bool is_noexcept_v = callable_traits<Fn>::is_noexcept;

} // namespace ac
