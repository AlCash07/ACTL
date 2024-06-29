// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/nesting_depth.hpp>
#include <tuple>
#include <utility>

namespace ac {

/// Concept of a tuple, that is a fixed size indexed sequence of potentially
/// heterogeneous types.
/// Similarly to `std::ranges::range`, types with qualifiers and supported.
// TODO: implement a full concept https://stackoverflow.com/a/68444475
template<class T>
concept Tuple =
    requires { std::tuple_size<std::remove_reference_t<T>>::value; };

template<Tuple T>
using tuple_indices_t =
    std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<T>>>;

template<Tuple T, class Seq = tuple_indices_t<T>>
struct tuple_nesting_depth;

template<Tuple T, size_t... Is>
struct tuple_nesting_depth<T, std::index_sequence<Is...>>
    : size_constant<
          1 + max_nesting_depth_v<
                  std::tuple_element_t<Is, std::remove_reference_t<T>>...>> {};

template<Tuple T>
struct nesting_depth<T> : tuple_nesting_depth<T> {};

} // namespace ac
