/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text.hpp>
#include <actl/std/string.hpp>

namespace ac::io {

/**
 * Format that inserts delimiter between consecutive output units.
 */
template <class Char = char>
struct spaced {
    struct format_tag;

    bool separate = false;
    std::basic_string<Char> space = " ";
};

template <class C, class T, enable_int_if<!is_manipulator<std::remove_const_t<T>>::value> = 0>
inline tuple<cspan<C>, T&> deserialize(spaced<C>& fmt, T& x) {
    if (fmt.separate) {
        return {span{fmt.space}, x};
    } else {
        fmt.separate = true;
        return {{}, x};
    }
}

template <class C, class T>
inline decltype(auto) deserialize(spaced<C>& fmt, const raw<T>& x) {
    fmt.separate = false;
    return x;
}

template <class C, class T>
inline auto serialize(spaced<C>& fmt, const T& x) -> decltype(deserialize(fmt, x)) {
    return deserialize(fmt, x);
}

struct setspace {
    std::string_view value;

    struct is_manipulator;
};

template <class C>
inline void serialize(spaced<C>& fmt, setspace x) {
    fmt.space = x.value;
}

template <class C>
inline void change_depth(spaced<C>& fmt, bool deeper) {
    fmt.separate = !deeper;
}

}  // namespace ac::io
