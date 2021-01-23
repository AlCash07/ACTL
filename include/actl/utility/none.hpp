// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

/// Empty class to enable void instantiation and take advantage of empty-base optimization.
struct none {
    template <class... Ts>
    explicit constexpr none(Ts...) {}

    using value_type = none;
};

// clang-format off
template <class T> struct replace_void             { using type = T;          };
template <>        struct replace_void<void>       { using type = none;       };
template <>        struct replace_void<const void> { using type = const none; };
// clang-format on

template <class T>
using replace_void_t = typename replace_void<T>::type;

}  // namespace ac
