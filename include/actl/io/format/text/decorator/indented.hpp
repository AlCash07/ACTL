// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/format/text/text.hpp>
#include <actl/io/utility/repeat.hpp>

namespace ac::io {

template <class Char = char>
struct indented
{
    struct format_tag;

    Char fill = ' ';
    Char endl = '\n';
    size_t count = 1;
    size_t level = 0;
    bool indent = false;
};

template <class C, class T, enable_int_if<!is_range_v<T>> = 0>
batch<raw<cspan<C>>, repeat<C>, T&> encode(indented<C>& fmt, T& x)
{
    if (!fmt.indent)
    {
        fmt.indent = true;
        return {raw{cspan<C>{}}, repeat<C>{}, x};
    }
    else
    {
        return {
            raw{cspan<C>{&fmt.endl, 1}},
            repeat<C>{fmt.fill, fmt.count * fmt.level},
            x};
    }
}

template <class C, class T>
decltype(auto) encode(indented<C>& fmt, const raw<T>& x)
{
    return x;
}

template <class C, bool Deeper>
void manipulate(indented<C>& fmt, change_level<Deeper>)
{
    if constexpr (Deeper)
        fmt.indent = true;
    fmt.level += Deeper ? 1 : -1;
}

} // namespace ac::io
