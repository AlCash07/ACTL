// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/hash.hpp>
#include <actl/sequence/tuple/comparison.hpp>

namespace ac {

template<typename T, size_t... Is>
constexpr size_t hash_tuple(T const& x, std::index_sequence<Is...>) {
    return hash_value(std::get<Is>(x)...);
}

template<typename... Ts>
constexpr size_t hash_value(std::tuple<Ts...> const& x) {
    return hash_tuple(x, std::index_sequence_for<Ts...>{});
}

} // namespace ac
