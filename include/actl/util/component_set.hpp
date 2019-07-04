/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

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

protected:
    /**
     * Finds the first components that accepts given arguments (it's required to exist) and returns
     * its output.
     */
    template <class... Ts>
    decltype(auto) execute_first(Ts&&... args) {
        return exec_first<0>(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    void execute_all(Ts&&... args) {
        exec_all<0>(std::forward<Ts>(args)...);
    }

private:
    using tuple_t = std::tuple<Components...>;

    template <size_t I, class... Ts>
    decltype(auto) exec_first(Ts&&... args) {
        static_assert(I < sizeof...(Components), "no component with requested overload");
        if constexpr (std::is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>) {
            return std::get<I>(components_)(std::forward<Ts>(args)...);
        } else {
            return exec_first<I + 1>(std::forward<Ts>(args)...);
        }
    }

    template <size_t I, class... Ts>
    void exec_all(Ts&&... args) {
        if constexpr (I < sizeof...(Components)) {
            if constexpr (std::is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>) {
                std::get<I>(components_)(std::forward<Ts>(args)...);
            }
            exec_all<I + 1>(std::forward<Ts>(args)...);
        }
    }

    tuple_t components_;
};

}  // namespace ac
