// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/utility/predicate.hpp>

namespace ac::io {

template<typename T, typename P>
struct till {
    explicit constexpr till(T x, P pred) : value{x}, terminator{pred} {}

    T value;
    predicate<P> terminator;
};

template<typename T, typename P>
till(T&&, P) -> till<T, P>;

template<typename P, size_t N>
size_t read_till(Device auto& id, till<span<char, N>, P> x) {
    size_t i = 0;
    size_t const size = x.value.size();
    for (; i < size; ++i) {
        auto c = id.get();
        if (id.eof() || x.terminator(c))
            break;
        x.value[i] = c;
    }
    return i;
}

template<typename P, size_t N>
size_t read_till(BufferedInputDevice auto& id, till<span<char, N>, P> x) {
    size_t i = 0;
    size_t const size = x.value.size();
    while (true) {
        auto s = id.input_buffer();
        auto end = std::min(s.end(), s.begin() + (size - i));
        auto ptr = s.begin();
        while (ptr != end && !x.terminator(*ptr))
            x.value[i++] = *ptr++;
        id.move(ptr - s.begin());
        if (i == size || s.empty() || ptr != end)
            break;
    }
    return i;
}

template<typename T, typename P>
bool read_final(Device auto& id, Format auto&, till<T, P> x) {
    read_till(id, x);
    return true;
}

} // namespace ac::io
