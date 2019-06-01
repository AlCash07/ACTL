/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>

namespace ac::io {

template <class Char>
struct raw {
    Char c;
};

template <class Char>
struct raw<span<const Char>> : span<const Char> {};

template <class T>
raw(T)->raw<T>;

template <class T>
struct is_manipulator<raw<T>> : std::true_type {};

template <class Device, class Format, class Char>
inline index serialize(Device& od, Format&, raw<Char> x) {
    return od.put(x.c);
}

template <class Device, class Format, class Char>
inline index serialize(Device& od, Format&, const raw<span<const Char>>& x) {
    return od.write(x);
}

}  // namespace ac::io
