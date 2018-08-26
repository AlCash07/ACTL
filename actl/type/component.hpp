/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <tuple>
#include <utility>

namespace ac {

/**
 * Base class for algorithm component. Inherited classes should overload operator() for the
 * appropriate arguments.
 */
struct component {
    struct not_implemented {};

    template <class... Ts>
    constexpr not_implemented operator()(Ts...) const { return {}; }
};

namespace detail {

template <size_t I, class Tuple, class... Ts>
inline auto exec_one(const Tuple& components, Ts&&... args) {
    static_assert(I < std::tuple_size_v<Tuple>, "no component with requested overload");
    auto result = std::get<I>(components)(std::forward<Ts>(args)...);
    if constexpr (std::is_same_v<decltype(result), component::not_implemented>) {
        return exec_one<I + 1>(components, std::forward<Ts>(args)...);
    } else {
        return result;
    }
}

template <size_t... Is, class Tuple, class... Ts>
inline void exec_all(std::index_sequence<Is...>, const Tuple& components, Ts&&... args) {
    (std::get<Is>(components)(std::forward<Ts>(args)...), ...);
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
    detail::exec_all(std::make_index_sequence<std::tuple_size_v<Tuple>>(), components,
                     std::forward<Ts>(args)...);
}

}  // namespace ac
