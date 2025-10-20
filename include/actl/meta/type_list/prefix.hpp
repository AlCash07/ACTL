// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/type_list/concat.hpp>

namespace ac {

template<class TypeList, size_t Length>
struct types_prefix;

/// TypeList prefix of a given Length as ac::type_list.
template<class TypeList, size_t Length>
using prefix_t = typename types_prefix<TypeList, Length>::type;

template<class... Types>
struct types_prefix<type_list<Types...>, 0> {
    using type = type_list<>;
};
template<class T0, class... Types>
struct types_prefix<type_list<T0, Types...>, 1> {
    using type = type_list<T0>;
};
template<class T0, class T1, class... Types>
struct types_prefix<type_list<T0, T1, Types...>, 2> {
    using type = type_list<T0, T1>;
};
template<class T0, class T1, class T2, class... Types>
struct types_prefix<type_list<T0, T1, T2, Types...>, 3> {
    using type = type_list<T0, T1, T2>;
};
template<class T0, class T1, class T2, class T3, class... Types>
struct types_prefix<type_list<T0, T1, T2, T3, Types...>, 4> {
    using type = type_list<T0, T1, T2, T3>;
};
template<class T0, class T1, class T2, class T3, class... Types, size_t Length>
    requires(Length >= 4)
struct types_prefix<type_list<T0, T1, T2, T3, Types...>, Length> {
    using type = concat_t<
        type_list<T0, T1, T2, T3>,
        prefix_t<type_list<Types...>, Length - 4>>;
};

} // namespace ac
