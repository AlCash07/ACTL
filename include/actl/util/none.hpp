/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac {

/**
 * Empty class to enable void instantiation and take advantage of empty-base optimization.
 */
struct none {
    template <class... Ts>
    explicit constexpr none(Ts&&...) {}
};

template <class T> struct replace_void             { using type = T;          };
template <>        struct replace_void<void>       { using type = none;       };
template <>        struct replace_void<const void> { using type = const none; };

template <class T>
using replace_void_t = typename replace_void<T>::type;

template <class Device>
inline bool read(Device&, none&) { return true; }

template <class Device>
inline int write(Device&, const none&) { return 0; }

}  // namespace ac