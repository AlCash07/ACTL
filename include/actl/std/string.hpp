// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/text/text.hpp>
#include <actl/io/utility/till.hpp>
#include <actl/range/traits.hpp>
#include <actl/string/ctype.hpp>
#include <string>

namespace ac {

template <class C, class T, class A>
struct range_traits<std::basic_string<C, T, A>> : default_range_traits
{
    static constexpr bool is_container = true;
};

namespace io {

template <class Device, class C, class T, class A, class P>
void read_till(Device& id, till<std::basic_string<C, T, A>&, P> x)
{
    index length = std::max(index{16}, static_cast<index>(x.value.capacity()));
    for (index last = 0;; length = last += length)
    {
        x.value.resize(static_cast<size_t>(last + length));
        auto* ptr = &x.value[static_cast<size_t>(last)];
        index chars_read = read_till(id, till{span{ptr, length}, x.terminator});
        if (chars_read < length)
        {
            x.value.resize(static_cast<size_t>(last + chars_read));
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

} // namespace io

} // namespace ac
