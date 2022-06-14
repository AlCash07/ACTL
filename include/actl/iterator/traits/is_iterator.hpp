// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/traits/dependent.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac {

namespace detail {

template <class T, class = void>
struct is_iter : std::false_type
{};

template <class T>
struct is_iter<T, void_t<iter_difference_t<T>, decltype(++std::declval<T&>())>>
    : std::true_type
{};

} // namespace detail

template <class T, class = void>
struct is_iterator : detail::is_iter<T>
{};

// https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator
template <class T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;

} // namespace ac
