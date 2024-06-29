// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template <class T>
struct associated_types {};

namespace detail {

// TODO: consider deducing const_reference.
#define AC_ASSOCIATED_TYPE_IMPL(name)                               \
    template <class T>                                              \
    struct name##_impl {};                                          \
                                                                    \
    template <class T>                                              \
    concept has_##name = !std::is_const_v<T>&&                      \
                             requires(T)                            \
    {                                                               \
        typename T::name;                                           \
    };                                                              \
                                                                    \
    template <has_##name T>                                         \
    struct name##_impl<T> {                                         \
        using type = typename T::name;                              \
    };                                                              \
                                                                    \
    template <class T>                                              \
        requires(!has_##name<T> && has_##name<associated_types<T>>) \
    struct name##_impl<T> {                                         \
        using type = typename associated_types<T>::name;            \
    };

AC_ASSOCIATED_TYPE_IMPL(value_type)
AC_ASSOCIATED_TYPE_IMPL(reference)

#undef AC_ASSOCIATED_TYPE_IMPL

} // namespace detail

template <class T>
using value_t =
    typename detail::value_type_impl<std::remove_reference_t<T>>::type;

template <class T>
using reference_t =
    typename detail::reference_impl<std::remove_reference_t<T>>::type;

// TODO: add element_t.

} // namespace ac
