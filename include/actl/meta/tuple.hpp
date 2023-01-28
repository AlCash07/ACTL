// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/nesting_depth.hpp>
#include <actl/meta/tuple.hpp>
#include <tuple>
#include <utility>

namespace ac {

template <class T>
using tuple_indices_t = std::make_index_sequence<std::tuple_size_v<T>>;

template <class T>
concept Tuple = requires { std::tuple_size<T>::value; };

namespace detail {

template <class T, class Seq>
struct tuple_depth;

template <class T, size_t... Is>
struct tuple_depth<T, std::index_sequence<Is...>>
    : size_constant<1 + max_nesting_depth_v<std::tuple_element_t<Is, T>...>>
{};

} // namespace detail

template <class T>
struct nesting_depth<T, std::enable_if_t<Tuple<T>>>
    : detail::tuple_depth<T, tuple_indices_t<T>>
{};

} // namespace ac
