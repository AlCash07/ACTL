// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iterator>

namespace ac {

template <class T, class = void>
struct is_range : std::false_type {};

template <class T>  // & is needed for C arrays
struct is_range<T,
                std::void_t<decltype(std::begin(std::declval<T&>()), std::end(std::declval<T&>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_range_v = is_range<T>::value;

template <class T, class = void>
struct is_contiguous_range : std::false_type {};

template <class T>  // & is needed for C arrays
struct is_contiguous_range<
    T, std::void_t<decltype(std::data(std::declval<T&>()), std::size(std::declval<T&>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_contiguous_range_v = is_contiguous_range<T>::value;

}  // namespace ac
