// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/deduce_noexcept_and_return.hpp>
#include <actl/functional/noexcept/deduce_noexcept_decltype_and_return.hpp>
#include <actl/meta/tuple.hpp>

namespace ac {

namespace detail {

template <class T, class... Args>
constexpr void invoke_if_can(T&&, Args&...) noexcept
{}

template <class T, class... Args>
    requires std::is_invocable_v<T, Args...>
constexpr auto invoke_if_can(T&& callable, Args&... args)
    // TODO: use std::invoke when it's constexpr.
    AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(std::forward<T>(callable)(args...))

template <size_t... Is, Tuple T, class... Args>
constexpr void invoke_all_matching_impl(
    std::index_sequence<Is...>, T&& tuple_to_invoke, Args&... args)
    AC_DEDUCE_NOEXCEPT_AND_RETURN((
        ...,
        invoke_if_can(std::get<Is>(std::forward<T>(tuple_to_invoke)), args...)))

} // namespace detail

/// Invokes in order all the tuple elements that are invocable with @p args.
/// @note @p args aren't forwarded to the invocations but passed by reference,
/// because they may be shared between multiple invocations.
template <Tuple T, class... Args>
constexpr void invoke_all_matching(T&& tuple_to_invoke, Args&&... args)
    AC_DEDUCE_NOEXCEPT_AND_RETURN(detail::invoke_all_matching_impl(
        tuple_indices_t<T>{}, std::forward<T>(tuple_to_invoke), args...))

} // namespace ac
