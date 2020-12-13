// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/util/predicate.hpp>

namespace ac::io {

template <class T, class P>
struct till {
    explicit constexpr till(T x, P pred) : value{x}, terminator{pred} {}

    T value;
    predicate<P> terminator;
};

template <class T, class P>
till(T&&, P) -> till<T, P>;

template <class Device, class P, index N>
index read_till(Device& id, till<span<char, N>, P> x) {
    index i = 0;
    const index size = x.value.size();
    if constexpr (has_input_buffer<Device>::value) {
        while (true) {
            auto s = id.input_data();
            auto end = std::min(s.end(), s.begin() + (size - i));
            auto ptr = s.begin();
            while (ptr != end && !x.terminator(*ptr)) x.value[i++] = *ptr++;
            id.move(ptr - s.begin());
            if (i == size || s.empty() || ptr != end) break;
        };
    } else {
        for (; i < size; ++i) {
            auto c = id.get();
            if (id.eof() || x.terminator(c)) break;
            x.value[i] = c;
        }
    }
    return i;
}

template <class Device, class Format, class T, class P>
bool read_final(Device& id, Format&, till<T, P> x) {
    read_till(id, x);
    return true;
}

}  // namespace ac::io
