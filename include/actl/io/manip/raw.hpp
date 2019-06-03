/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/span.hpp>

namespace ac::io {

template <class T>
struct raw {
    T value;
};

template <class T>
raw(T)->raw<T>;

template <class T>
struct is_raw : std::false_type {};

template <class T>
struct is_raw<raw<T>> : std::true_type {};

template <class Device, class Format, class Char>
inline index serialize(Device& od, Format&, raw<Char> x) {
    return od.put(x.value);
}

template <class Device, class Format, class Char>
inline index serialize(Device& od, Format&, const raw<cspan<Char>>& x) {
    return od.write(x.value);
}

}  // namespace ac::io
