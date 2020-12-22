// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/traits/is_range.hpp>
#include <utility>

namespace ac {

template <class T, bool = is_range_v<T>, class = void>
struct is_tuple : std::false_type {};

template <class T>
struct is_tuple<T, false, std::void_t<decltype(std::tuple_size<T>::value)>> : std::true_type {};

template <class T>
constexpr bool is_tuple_v = is_tuple<T>::value;

}  // namespace ac
