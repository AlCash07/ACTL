// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_traits.hpp>
#include <tuple>

namespace ac {

/// Finds the first function that accepts given arguments (it's required to
/// exist) and returns its output.
template <size_t I = 0, class Tuple, class... Args>
constexpr decltype(auto) invoke_first_matching(
    Tuple&& tuple_to_invoke, const Args&... args)
{
    using T = remove_cvref_t<Tuple>;
    static_assert(
        I < std::tuple_size_v<T>, "no element with requested signature");
    if constexpr (std::is_invocable_v<std::tuple_element_t<I, T>, Args...>)
        // TODO: use std::invoke when it's constexpr.
        return std::get<I>(tuple_to_invoke)(args...);
    else
        return invoke_first_matching<I + 1>(tuple_to_invoke, args...);
}

template <size_t I = 0, class Tuple, class... Args>
constexpr void invoke_all_matching(
    [[maybe_unused]] Tuple&& tuple_to_invoke,
    [[maybe_unused]] const Args&... args)
{
    using T = remove_cvref_t<Tuple>;
    if constexpr (I < std::tuple_size_v<T>)
    {
        if constexpr (std::is_invocable_v<std::tuple_element_t<I, T>, Args...>)
            // TODO: use std::invoke when it's constexpr.
            std::get<I>(tuple_to_invoke)(args...);
        invoke_all_matching<I + 1>(tuple_to_invoke, args...);
    }
}

} // namespace ac
