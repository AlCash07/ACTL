// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/tuple/comparison.hpp>
#include <actl/io/tuple.hpp>

namespace ac {

template <class T, size_t... Is>
constexpr size_t hash_tuple(const T& x, std::index_sequence<Is...>) {
    return hash_value(std::get<Is>(x)...);
}

template <class... Ts>
constexpr size_t hash_value(const std::tuple<Ts...>& x) {
    return hash_tuple(x, std::make_index_sequence<sizeof...(Ts)>{});
}

}  // namespace ac
