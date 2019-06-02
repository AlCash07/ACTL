/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/span.hpp>

namespace ac::io {

// Use this class to write string representation of a non-string type.
// Distinguishes such string from spans passed by the user.
template <class Char>
class char_span : public span<const Char> {
public:
    using span<const Char>::span;

    template <index N>
    char_span(const Char (&array)[N]) : span<const Char>{array, array[N - 1] ? N : N - 1} {}
};

template <class Char>
struct raw {
    Char c;
};

template <class Char>
struct raw<char_span<Char>> : char_span<Char> {};

template <class T>
raw(T)->raw<T>;

template <class T>
struct is_raw : std::false_type {};

template <class T>
struct is_raw<raw<T>> : std::true_type {};

template <class Device, class Format, class Char>
inline index serialize(Device& od, Format&, raw<Char> x) {
    return od.put(x.c);
}

template <class Device, class Format, class Char>
inline index serialize(Device& od, Format&, const raw<char_span<Char>>& x) {
    return od.write(x);
}

}  // namespace ac::io
