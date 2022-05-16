// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/format/text/text.hpp>
#include <actl/io/utility/till.hpp>
#include <actl/string/char_classes.hpp>

namespace ac::io {

template <class Device, class C, class T, class A, class P>
void read_till(Device& id, till<std::basic_string<C, T, A>&, P> x)
{
    size_t length = std::max(size_t{16}, x.value.capacity());
    for (size_t last = 0;; length = last += length)
    {
        x.value.resize(last + length);
        auto* ptr = &x.value[last];
        size_t chars_read =
            read_till(id, till{span{ptr, length}, x.terminator});
        if (chars_read < length)
        {
            x.value.resize(last + chars_read);
            return;
        }
    }
}

template <
    class Format,
    class C,
    class T,
    class A,
    enable_int_if_text<Format> = 0>
auto make_parser(Format&, std::basic_string<C, T, A>& x)
{
    return till{x, is_space};
}

} // namespace ac::io
