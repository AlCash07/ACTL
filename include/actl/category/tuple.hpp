// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/category/utility/nesting_depth.hpp>
#include <tuple>
#include <utility>

namespace ac {

template <class T>
using tuple_indices_t = std::make_index_sequence<std::tuple_size_v<T>>;

template <class T, bool = is_range_v<T>, class = void>
struct is_tuple : std::false_type {};

template <class T>
struct is_tuple<T, false, std::void_t<decltype(std::tuple_size<T>::value)>>
    : std::true_type {};

template <class T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

struct tuple_tag {
    using base = unclassified_tag;
};

template <class T>
struct category_sfinae<T, std::enable_if_t<is_tuple_v<T>>> {
    using type = tuple_tag;
};

namespace detail {

template <class T, class Seq>
struct tuple_depth;

template <class T, size_t... Is>
struct tuple_depth<T, std::index_sequence<Is...>>
    : index_constant<1 + max_nesting_depth_v<std::tuple_element_t<Is, T>...>> {
};

} // namespace detail

template <class T>
struct nesting_depth<T, std::enable_if_t<is_tuple_v<T>>>
    : detail::tuple_depth<T, tuple_indices_t<T>> {};

} // namespace ac
