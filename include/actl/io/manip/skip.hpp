/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/manip/predicate.hpp>
#include <actl/traits/ctype.hpp>

namespace ac::io {

template <class T>
struct skip : public predicate<T> {
    explicit constexpr skip(T value) : predicate<T>{value} {}
};

inline constexpr auto ws = skip{is_space};  // skip whitespace

template <class Device, class Format, class T>
inline bool deserialize(Device& id, Format&, skip<T> x) {
    while (!id.eof() && x(id.peek())) id.move(1);
    return true;
}

}  // namespace ac::io
