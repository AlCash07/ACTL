// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/sequence/type_array/type_array.hpp>

namespace ac {

template<typename... TypeLists>
struct types_concat;

/// Concatenation of the given TypeLists as a single ac::type_array.
template<typename... TypeLists>
using concat_t = typename types_concat<TypeLists...>::type;

template<typename... Types>
struct types_concat<type_array<Types...>> {
    using type = type_array<Types...>;
};

template<>
struct types_concat<> {
    using type = type_array<>;
};

template<typename... Types0, typename... Types1, typename... TypeLists>
struct types_concat<
    type_array<Types0...>,
    type_array<Types1...>,
    TypeLists...> {
    using type = concat_t<type_array<Types0..., Types1...>, TypeLists...>;
};

} // namespace ac
