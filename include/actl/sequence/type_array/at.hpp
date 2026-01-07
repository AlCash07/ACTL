// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/sequence/type_array/type_array.hpp>

namespace ac {

template<typename TypeList, size_t Index>
struct type_at;

/// Type at the given Index in a TypeList.
template<typename TypeList, size_t Index>
using at_t = typename type_at<TypeList, Index>::type;

// Recursion with step 8 is used to simplify the code,
// because larger steps don't provide
// significant compilation time improvements.
// For reference, see:
// https://ldionne.com/2015/11/29/efficient-parameter-pack-indexing/
// https://marzer.github.io/md_blog_2021_05_31_compilation_speed_humps_std_tuple.html
//
// TODO: use Clang's __type_pack_element builtin.
// The difficulty is in reliably checking Clang version for it.
template<typename T0, typename... Types>
struct type_at<type_array<T0, Types...>, 0> {
    using type = T0;
};
template<typename T0, typename T1, typename... Types>
struct type_at<type_array<T0, T1, Types...>, 1> {
    using type = T1;
};
template<typename T0, typename T1, typename T2, typename... Types>
struct type_at<type_array<T0, T1, T2, Types...>, 2> {
    using type = T2;
};
template<typename T0, typename T1, typename T2, typename T3, typename... Types>
struct type_at<type_array<T0, T1, T2, T3, Types...>, 3> {
    using type = T3;
};
template<
    typename T0,
    typename T1,
    typename T2,
    typename T3,
    typename T4,
    typename... Types>
struct type_at<type_array<T0, T1, T2, T3, T4, Types...>, 4> {
    using type = T4;
};
// clang-format off
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename... Types>
struct type_at<type_array<T0, T1, T2, T3, T4, T5, Types...>, 5> {
    using type = T5;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename... Types>
struct type_at<type_array<T0, T1, T2, T3, T4, T5, T6, Types...>, 6> {
    using type = T6;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename... Types>
struct type_at<type_array<T0, T1, T2, T3, T4, T5, T6, T7, Types...>, 7> {
    using type = T7;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename... Types, size_t Index>
    requires(Index >= 8)
struct type_at<type_array<T0, T1, T2, T3, T4, T5, T6, T7, Types...>, Index> {
    using type = at_t<type_array<Types...>, Index - 8>;
};
// clang-format on

} // namespace ac
