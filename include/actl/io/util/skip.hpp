/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/util/predicate.hpp>
#include <actl/string/ctype.hpp>

namespace ac::io {

template <class T, class Char = char>
inline constexpr bool is_string_v = std::is_convertible_v<T, std::basic_string_view<Char>>;

template <class T>
struct skip : public predicate<T> {
    explicit constexpr skip(T value) : predicate<T>{value} {}
};

inline constexpr auto ws = skip{is_space};  // skip whitespace

template <class Device, class Format, class T>
inline bool read_final(Device& id, Format&, skip<T> x) {
    while (!id.eof() && x(id.peek())) id.move(1);
    return true;
}

struct skipws {
    struct format_tag;
};

template <class T, enable_int_if<std::is_arithmetic_v<T> || is_string_v<T>> = 0>
inline auto deserialize(skipws&, T& x) {
    return tuple{ws, x};
}

}  // namespace ac::io
