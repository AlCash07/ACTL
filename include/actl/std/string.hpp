/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/traits.hpp>
#include <actl/io/text/text.hpp>
#include <actl/io/util/till.hpp>
#include <actl/string/ctype.hpp>
#include <string>

namespace ac {

template <class C, class T, class A>
struct container_category<std::basic_string<C, T, A>> : contiguous_container_tag {};

namespace io {

template <class Device, class Format, class T, class A, class P, enable_int_if_text<Format> = 0>
inline bool deserialize(Device& id, Format& fmt,
                        io::till<std::basic_string<io::char_t<Device>, T, A>&, P> x) {
    index length = std::max(index{16}, static_cast<index>(x.value.capacity()));
    for (index last = 0;; length = last += length) {
        x.value.resize(static_cast<size_t>(last + length));
        auto* ptr = &x.value[static_cast<size_t>(last)];
        index chars_read = io::read_till(id, io::till{span{ptr, length}, x.terminator});
        if (chars_read < length) {
            x.value.resize(static_cast<size_t>(last + chars_read));
            return true;
        }
    }
}

template <class Device, class Format, class T, class A, enable_int_if_text<Format> = 0>
inline bool deserialize(Device& id, Format& fmt, std::basic_string<io::char_t<Device>, T, A>& x) {
    return deserialize(id, fmt, io::till{x, is_space});
}

}  // namespace io

}  // namespace ac
