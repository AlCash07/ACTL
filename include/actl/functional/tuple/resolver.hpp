// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/tuple.hpp>

namespace ac {

template <class T, class... Ts>
struct tuple_op_resolver : tuple_op_resolver<tuple_indices_t<T>, T, Ts...> {};

template <size_t... Is, class T, class U>
struct tuple_op_resolver<std::index_sequence<Is...>, T, U> {
    static_assert(std::tuple_size_v<T> == std::tuple_size_v<U>);

    template <class Composer, class Op>
    static constexpr auto resolve(Composer composer, const Op& op) {
        return composer(
            op.template resolve<std::tuple_element_t<Is, T>, std::tuple_element_t<Is, U>>()...);
    }
};

}  // namespace ac
