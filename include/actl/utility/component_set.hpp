// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/std/tuple.hpp>
#include <type_traits>

namespace ac {

template <class... Components>
class component_set {
public:
    component_set() = default;

    explicit component_set(Components&&... components)
        : components_{std::forward<Components>(components)...} {}

    /// Finds the first components that accepts given arguments (it's required to exist) and returns
    /// its output.
    template <class... Ts>
    constexpr decltype(auto) execute_first(const Ts&... xs) {
        return exec_first<0>(xs...);
    }

    template <class... Ts>
    constexpr void execute_all(const Ts&... xs) {
        exec_all<0>(xs...);
    }

private:
    using tuple_t = std::tuple<Components...>;

    template <size_t I, class... Ts>
    constexpr decltype(auto) exec_first(const Ts&... xs) {
        static_assert(I < sizeof...(Components), "no component with requested overload");
        if constexpr (std::is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>)
            return std::get<I>(components_)(xs...);
        else
            return exec_first<I + 1>(xs...);
    }

    template <size_t I, class... Ts>
    constexpr void exec_all(const Ts&... xs) {
        if constexpr (I < sizeof...(Components)) {
            if constexpr (std::is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>)
                std::get<I>(components_)(xs...);
            exec_all<I + 1>(xs...);
        }
    }

    tuple_t components_;
};

template <class... Ts>
component_set(Ts&&...) -> component_set<Ts...>;

}  // namespace ac
