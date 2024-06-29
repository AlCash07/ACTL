// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/deduce_noexcept_and_return.hpp>
#include <actl/meta/tuple.hpp>

namespace ac {

namespace detail {

template <size_t I, Tuple T, class... Args>
constexpr size_t find_first_matching() noexcept {
    static_assert(
        I < std::tuple_size_v<T>, "no element with requested signature"
    );
    if constexpr (std::is_invocable_v<std::tuple_element_t<I, T>, Args...>)
        return I;
    else
        return find_first_matching<I + 1, T, Args...>();
}

} // namespace detail

/// Invokes the first tuple element that is invocable with @p args
/// (it's required to exist) and returns its result.
template <Tuple T, class... Args>
constexpr decltype(auto) invoke_first_matching(
    T&& tuple_to_invoke, Args&&... args
)
    // TODO: use std::invoke when it's constexpr.
    AC_DEDUCE_NOEXCEPT_AND_RETURN(
        std::get<
            detail::find_first_matching<0, std::remove_cvref_t<T>, Args...>(
            )>(std::forward<T>(tuple_to_invoke))(std::forward<Args>(args)...)
    )

} // namespace ac
