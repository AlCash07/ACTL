// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

// TODO: remove this in C++20 and just use `requires`.
#define AC_DEFINE_HAS_MEMBER_F(name)                                          \
    template <class T, class = void>                                          \
    struct has_member_f_##name : std::false_type                              \
    {};                                                                       \
                                                                              \
    template <class T>                                                        \
    struct has_member_f_##name<T, void_t<decltype(std::declval<T>().name())>> \
        : std::true_type                                                      \
    {};                                                                       \
                                                                              \
    template <class T>                                                        \
    inline constexpr bool has_member_f_##name##_v =                           \
        has_member_f_##name<T>::value;

#define AC_DEFINE_HAS_NON_MEMBER_F(name)                            \
    template <class T, class = void>                                \
    struct has_non_member_f_##name : std::false_type                \
    {};                                                             \
                                                                    \
    template <class T>                                              \
    struct has_non_member_f_##name<                                 \
        T,                                                          \
        void_t<decltype(name(std::declval<T>()))>> : std::true_type \
    {};                                                             \
                                                                    \
    template <class T>                                              \
    inline constexpr bool has_non_member_f_##name##_v =             \
        has_non_member_f_##name<T>::value;
