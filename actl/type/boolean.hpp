/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac {

/**
 * Bool wrapper to avoid using std::vector<bool> specialization.
 */
struct boolean {
    constexpr boolean() : value{} {}
    constexpr boolean(bool value) : value{value} {}

    constexpr operator bool&       ()       { return value; }
    constexpr operator const bool& () const { return value; }

    bool value;
};

template <class T> struct replace_bool             { using type = T;             };
template <>        struct replace_bool<bool>       { using type = boolean;       };
template <>        struct replace_bool<const bool> { using type = const boolean; };

template <class T>
using replace_bool_t = typename replace_bool<T>::type;

template <class Device>
inline bool read(Device& in, boolean& arg) { return read(in, arg.value); }

template <class Device>
inline int write(Device& out, const boolean& arg) { return write(out, arg.value); }

}  // namespace ac
