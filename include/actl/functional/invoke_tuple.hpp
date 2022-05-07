// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/type_traits.hpp>
#include <cstddef>
#include <tuple>

namespace ac {

namespace detail {

template <size_t I, class Tuple, class... Args>
constexpr size_t find_first_matching() noexcept
{
    static_assert(
        I < std::tuple_size_v<Tuple>, "no element with requested signature");
    if constexpr (std::is_invocable_v<std::tuple_element_t<I, Tuple>, Args...>)
        return I;
    else
        return find_first_matching<I + 1, Tuple, Args...>();
}

template <
    class T,
    class... Args,
    enable_int_if<!std::is_invocable_v<T, Args...>> = 0>
constexpr void invoke_if_can(T&&, Args&...) noexcept
{}

template <class T, class... Args>
constexpr auto invoke_if_can(T&& callable, Args&... args)
    // TODO: use std::invoke when it's constexpr.
    AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(std::forward<T>(callable)(args...))

template <size_t... Is, class Tuple, class... Args>
constexpr void invoke_all_matching_impl(
    std::index_sequence<Is...>, Tuple&& tuple_to_invoke, Args const&... args)
    AC_DEDUCE_NOEXCEPT_AND_RETURN(
        (...,
         invoke_if_can(
             std::get<Is>(std::forward<Tuple>(tuple_to_invoke)), args...)))

} // namespace detail

/// Invokes the first tuple element that is invocable with @p args
/// (it's required to exist) and returns its result.
template <class Tuple, class... Args>
constexpr decltype(auto) invoke_first_matching(
    Tuple&& tuple_to_invoke, Args&&... args)
    // TODO: use std::invoke when it's constexpr.
    AC_DEDUCE_NOEXCEPT_AND_RETURN(
        std::get<
            detail::find_first_matching<0, remove_cvref_t<Tuple>, Args...>()>(
            std::forward<Tuple>(tuple_to_invoke))(std::forward<Args>(args)...))

/// Invokes in order all the tuple elements that are invocable with @p args.
/// @note @p args aren't forwarded to the invocations but passed by reference,
/// because they may be shared between multiple invocations.
template <class Tuple, class... Args>
constexpr void invoke_all_matching(Tuple&& tuple_to_invoke, Args&&... args)
    AC_DEDUCE_NOEXCEPT_AND_RETURN(detail::invoke_all_matching_impl(
        std::make_index_sequence<std::tuple_size_v<remove_cvref_t<Tuple>>>{},
        std::forward<Tuple>(tuple_to_invoke),
        args...))

} // namespace ac
