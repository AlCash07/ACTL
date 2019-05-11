/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/manip/predicate.hpp>

namespace ac::io {

template <class Char, class T>
class till : public span<Char> {
public:
    explicit constexpr till(span<Char> s, T pred) : span<Char>{s}, terminator{pred} {}

    predicate<T> terminator;
};

template <class Device, class Format, class T>
inline bool deserialize(Device& id, Format&, till<char_t<Device>, T> x) {
    // TODO: design buffer class and move read implementation here.
    id.read(x);
    return !id.eof();
}

}  // namespace ac::io
