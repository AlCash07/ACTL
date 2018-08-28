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

template <class... Components>
class algorithm {
    using tuple_t = std::tuple<Components...>;
    tuple_t components_;

    template <size_t I, class... Ts>
    auto exec_one(Ts&&... args) {
        static_assert(I < sizeof...(Components), "no component with requested overload");
        if constexpr (is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>) {
            return std::get<I>(components_)(std::forward<Ts>(args)...);
        } else {
            return exec_one<I + 1>(std::forward<Ts>(args)...);
        }
    }

    template <size_t I, class... Ts>
    void exec_all(Ts&&... args) {
        if constexpr (I < sizeof...(Components)) {
            if constexpr (is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>) {
                std::get<I>(components_)(std::forward<Ts>(args)...);
            }
            exec_all<I + 1>(std::forward<Ts>(args)...);
        }
    }

protected:
    /**
     * Finds the first components that accepts given arguments (it's required to exits) and returns
     * its output.
     */
    template <class... Ts>
    auto execute_one(Ts&&... args) {
        return exec_one<0>(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    void execute_all(Ts&&... args) {
        exec_all<0>(std::forward<Ts>(args)...);
    }

public:
    explicit algorithm(Components... components) : components_(components...) {}
};

}  // namespace ac
