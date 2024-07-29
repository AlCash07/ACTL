// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/free_function.hpp>
#include <actl/functional/category/member_function.hpp>
#include <actl/functional/traits/detail/function_object.hpp>
#include <type_traits>

namespace ac {

namespace detail {

// Type qualifiers and pointer don't matter for regular functions.
template<class Fn>
    requires FreeFunction<std::remove_pointer_t<std::remove_cvref_t<Fn>>>
struct function_traits<Fn>
    : free_function_traits<std::remove_pointer_t<std::remove_cvref_t<Fn>>> {};

template<class Fn>
    requires MemberFunction<std::remove_cvref_t<Fn>>
struct function_traits<Fn> : member_function_traits<std::remove_cvref_t<Fn>> {};

} // namespace detail

/* convenience aliases */

template<class Fn>
using return_t = typename detail::function_traits<Fn>::return_type;

template<class Fn>
inline constexpr size_t arity_v = detail::function_traits<Fn>::arity;

template<size_t Index, class Fn>
using parameter_at_t =
    typename detail::function_traits<Fn>::template parameter_at<Index>;

template<class Fn>
inline constexpr bool accepts_variadic_arguments_v =
    detail::function_traits<Fn>::accepts_variadic_arguments;

template<class Fn>
inline constexpr bool is_noexcept_v = detail::function_traits<Fn>::is_noexcept;

} // namespace ac
