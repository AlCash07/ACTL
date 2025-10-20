// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/type_list/type_list.hpp>

namespace ac {

template<class... TypeLists>
struct types_concat;

/// Concatenation of the given TypeLists as a single ac::type_list.
template<class... TypeLists>
using concat_t = typename types_concat<TypeLists...>::type;

template<class... Types>
struct types_concat<type_list<Types...>> {
    using type = type_list<Types...>;
};

template<>
struct types_concat<> {
    using type = type_list<>;
};

template<class... Types0, class... Types1, class... TypeLists>
struct types_concat<type_list<Types0...>, type_list<Types1...>, TypeLists...> {
    using type = concat_t<type_list<Types0..., Types1...>, TypeLists...>;
};

} // namespace ac
