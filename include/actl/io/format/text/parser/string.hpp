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

template<typename C, typename T, typename A, typename P>
void read_till(Device auto& id, till<std::basic_string<C, T, A>&, P> x) {
    size_t length = std::max(size_t{16}, x.value.capacity());
    for (size_t last = 0;; length = last += length) {
        x.value.resize(last + length);
        auto* ptr = &x.value[last];
        size_t chars_read =
            read_till(id, till{span{ptr, length}, x.terminator});
        if (chars_read < length) {
            x.value.resize(last + chars_read);
            return;
        }
    }
}

template<typename C, typename T, typename A>
auto make_parser(TextFormat auto&, std::basic_string<C, T, A>& x) {
    return till{x, is_space};
}

} // namespace ac::io
