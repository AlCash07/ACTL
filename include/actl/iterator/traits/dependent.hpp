// Analog of https://en.cppreference.com/w/cpp/iterator/iter_t
//
// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_traits.hpp>
#include <iterator>

namespace ac {

namespace detail {

template <class T, class = void>
struct iter_value
{
    // Standard version is too complicated for only iterators
    // https://en.cppreference.com/w/cpp/iterator/indirectly_readable_traits
    using type = typename T::value_type;
};

template <class T>
struct iter_value<T, std::void_t<typename std::iterator_traits<T>::value_type>>
{
    using type = typename std::iterator_traits<T>::value_type;
};

template <class T, class = void>
struct iter_difference
{
    // https://en.cppreference.com/w/cpp/iterator/incrementable_traits
    using type =
        std::make_signed_t<decltype(std::declval<T>() - std::declval<T>())>;
};

template <class T>
struct iter_difference<
    T,
    std::void_t<typename std::iterator_traits<T>::difference_type>>
{
    using type = typename std::iterator_traits<T>::difference_type;
};

} // namespace detail

template <class T>
using iter_value_t = typename detail::iter_value<remove_cvref_t<T>>::type;

template <class T>
using iter_reference_t = decltype(*std::declval<T&>());

template <class T>
using iter_difference_t =
    typename detail::iter_difference<remove_cvref_t<T>>::type;

} // namespace ac
