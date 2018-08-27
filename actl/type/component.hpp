/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/type_traits.hpp>
#include <tuple>

namespace ac {
namespace detail {

template <size_t I, class Tuple, class... Ts>
inline auto exec_one(const Tuple& components, Ts&&... args) {
    static_assert(I < std::tuple_size_v<Tuple>, "no component with requested overload");
    if constexpr (is_invocable_v<std::tuple_element_t<I, Tuple>, Ts...>) {
        return std::get<I>(components)(std::forward<Ts>(args)...);
    } else {
        return exec_one<I + 1>(components, std::forward<Ts>(args)...);
    }
}

template <size_t I, class Tuple, class... Ts>
inline void exec_all(const Tuple& components, Ts&&... args) {
    if constexpr (is_invocable_v<std::tuple_element_t<I, Tuple>, Ts...>) {
        std::get<I>(components)(std::forward<Ts>(args)...);
    }
    if constexpr (I + 1 < std::tuple_size_v<Tuple>) {
        exec_all<I + 1>(components, std::forward<Ts>(args)...);
    }
}

}  // namespace detail

/**
 * Finds the first components that accepts given arguments (it's required to exits) and returns its
 * output.
 */
template <class Tuple, class... Ts>
inline auto execute_one(const Tuple& components, Ts&&... args) {
    return detail::exec_one<0>(components, std::forward<Ts>(args)...);
}

template <class Tuple, class... Ts>
inline void execute_all(const Tuple& components, Ts&&... args) {
    detail::exec_all<0>(components, std::forward<Ts>(args)...);
}

}  // namespace ac
