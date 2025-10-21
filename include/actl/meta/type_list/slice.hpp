// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/type_list/concat.hpp>

namespace ac {

template<class TypeList, size_t First, size_t Last>
struct types_slice;

/// TypeList slice between the given indices as ac::type_list.
template<class TypeList, size_t First, size_t Last>
using slice_t = typename types_slice<TypeList, First, Last>::type;

/// TypeList prefix of a given Length as ac::type_list.
template<class TypeList, size_t Length>
using prefix_t = slice_t<TypeList, 0, Length>;

/// TypeList suffix of a given Length as ac::type_list.
template<class TypeList, size_t Length>
using suffix_t = slice_t<TypeList, TypeList::length - Length, TypeList::length>;

/* collecting types into a slice after the first index reaches 0 */
template<class... Types>
struct types_slice<type_list<Types...>, 0, 0> {
    using type = type_list<>;
};
template<class T0, class... Types>
struct types_slice<type_list<T0, Types...>, 0, 1> {
    using type = type_list<T0>;
};
template<class T0, class T1, class... Types>
struct types_slice<type_list<T0, T1, Types...>, 0, 2> {
    using type = type_list<T0, T1>;
};
template<class T0, class T1, class T2, class... Types>
struct types_slice<type_list<T0, T1, T2, Types...>, 0, 3> {
    using type = type_list<T0, T1, T2>;
};
template<class T0, class T1, class T2, class T3, class... Types, size_t Length>
    requires(Length >= 4)
struct types_slice<type_list<T0, T1, T2, T3, Types...>, 0, Length> {
    using type = concat_t<
        type_list<T0, T1, T2, T3>,
        prefix_t<type_list<Types...>, Length - 4>>;
};

/* dropping types until the first index reaches 0 */
template<class T0, class... Types, size_t Last>
struct types_slice<type_list<T0, Types...>, 1, Last> {
    using type = prefix_t<type_list<Types...>, Last - 1>;
};
template<class T0, class T1, class... Types, size_t Last>
struct types_slice<type_list<T0, T1, Types...>, 2, Last> {
    using type = prefix_t<type_list<Types...>, Last - 2>;
};
template<class T0, class T1, class T2, class... Types, size_t Last>
struct types_slice<type_list<T0, T1, T2, Types...>, 3, Last> {
    using type = prefix_t<type_list<Types...>, Last - 3>;
};
// clang-format off
template<class T0, class T1, class T2, class T3, class... Types, size_t First, size_t Last>
    requires(First >= 4)
struct types_slice<type_list<T0, T1, T2, T3, Types...>, First, Last> {
    using type = slice_t<type_list<Types...>, First - 4, Last - 4>;
};
// clang-format on

} // namespace ac
