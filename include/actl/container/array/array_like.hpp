// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/static_size.hpp>
#include <type_traits>
#include <utility>

namespace ac {

namespace detail {

template <class T, class Seq>
struct has_constant_indices : std::false_type {};

template <class T, size_t... Is>
    requires(... && requires(T t) { t[ac::constant<Is>{}]; })
struct has_constant_indices<T, std::index_sequence<Is...>> : std::true_type {};

} // namespace detail

template <class T>
concept ConstantIndexed =
    static_size_v<T> != dynamic_size &&
    detail::has_constant_indices<
        T,
        std::make_index_sequence<static_size_v<T>>>::value;

template <class T>
concept DynamicallyIndexed = static_size_v<T> == dynamic_size && requires(T t) {
    t.size();
    t[size_t{}];
};

/// We define the array-like concept as something with size and subscript
/// operator to index the elements from 0 to size-1.
/// Elements don't have to be of the same type, otherwise this would be just a
/// random access range. A tuple also fits this concept if it implements
/// a subscript operator taking `ac::constant`.
/// Names like "sequence" don't fit here, because types like linked list are
/// also sequences, but don't support fast indexing.
template <class T>
concept ArrayLike = ConstantIndexed<T> || DynamicallyIndexed<T>;

} // namespace ac
