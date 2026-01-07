// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/sequence/type_array/concat.hpp>

namespace ac {

template<typename TypeList, size_t First, size_t Last>
struct types_slice;

/// TypeList slice between the given indices as ac::type_array.
template<typename TypeList, size_t First, size_t Last>
using slice_t = typename types_slice<TypeList, First, Last>::type;

/// TypeList prefix of a given Length as ac::type_array.
template<typename TypeList, size_t Length>
using prefix_t = slice_t<TypeList, 0, Length>;

/// TypeList suffix of a given Length as ac::type_array.
template<typename TypeList, size_t Length>
using suffix_t = slice_t<TypeList, TypeList::length - Length, TypeList::length>;

/* collecting types into a slice after the first index reaches 0 */
template<typename... Types>
struct types_slice<type_array<Types...>, 0, 0> {
    using type = type_array<>;
};
template<typename T0, typename... Types>
struct types_slice<type_array<T0, Types...>, 0, 1> {
    using type = type_array<T0>;
};
template<typename T0, typename T1, typename... Types>
struct types_slice<type_array<T0, T1, Types...>, 0, 2> {
    using type = type_array<T0, T1>;
};
template<typename T0, typename T1, typename T2, typename... Types>
struct types_slice<type_array<T0, T1, T2, Types...>, 0, 3> {
    using type = type_array<T0, T1, T2>;
};
template<
    typename T0,
    typename T1,
    typename T2,
    typename T3,
    typename... Types,
    size_t Length>
    requires(Length >= 4)
struct types_slice<type_array<T0, T1, T2, T3, Types...>, 0, Length> {
    using type = concat_t<
        type_array<T0, T1, T2, T3>,
        prefix_t<type_array<Types...>, Length - 4>>;
};

/* dropping types until the first index reaches 0 */
template<typename T0, typename... Types, size_t Last>
struct types_slice<type_array<T0, Types...>, 1, Last> {
    using type = prefix_t<type_array<Types...>, Last - 1>;
};
template<typename T0, typename T1, typename... Types, size_t Last>
struct types_slice<type_array<T0, T1, Types...>, 2, Last> {
    using type = prefix_t<type_array<Types...>, Last - 2>;
};
template<typename T0, typename T1, typename T2, typename... Types, size_t Last>
struct types_slice<type_array<T0, T1, T2, Types...>, 3, Last> {
    using type = prefix_t<type_array<Types...>, Last - 3>;
};
// clang-format off
template<typename T0, typename T1, typename T2, typename T3, typename... Types, size_t First, size_t Last>
    requires(First >= 4)
struct types_slice<type_array<T0, T1, T2, T3, Types...>, First, Last> {
    using type = slice_t<type_array<Types...>, First - 4, Last - 4>;
};
// clang-format on

} // namespace ac
