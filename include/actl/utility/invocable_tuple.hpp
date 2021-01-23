// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <tuple>
#include <type_traits>

namespace ac {

template <class... Funcs>
class invocable_tuple {
    using tuple_t = std::tuple<Funcs...>;

    tuple_t functions_;

public:
    invocable_tuple() = default;

    constexpr explicit invocable_tuple(Funcs&&... functions)
        : functions_{std::forward<Funcs>(functions)...} {}

    /// Finds the first function that accepts given arguments (it's required to exist) and returns
    /// its output.
    template <size_t I = 0, class... Ts>
    constexpr decltype(auto) invoke_first(const Ts&... xs) {
        static_assert(I < sizeof...(Funcs), "no component with requested overload");
        if constexpr (std::is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>)
            return std::get<I>(functions_)(xs...);
        else
            return invoke_first<I + 1>(xs...);
    }

    template <size_t I = 0, class... Ts>
    constexpr void invoke_all([[maybe_unused]] const Ts&... xs) {
        if constexpr (I < sizeof...(Funcs)) {
            if constexpr (std::is_invocable_v<std::tuple_element_t<I, tuple_t>, Ts...>)
                std::get<I>(functions_)(xs...);
            invoke_all<I + 1>(xs...);
        }
    }
};

template <class... Funcs>
invocable_tuple(Funcs&&...) -> invocable_tuple<Funcs...>;

}  // namespace ac
