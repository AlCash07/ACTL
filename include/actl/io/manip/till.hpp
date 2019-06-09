/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/manip/predicate.hpp>

namespace ac::io {

template <class Char, class T>
class till : public span<Char> {
public:
    explicit constexpr till(span<Char> s, T pred) : span<Char>{s}, terminator{pred} {}

    predicate<T> terminator;
};

template <class Device, class Format, class T, class C = char_t<Device>>
inline bool deserialize(Device& id, Format&, till<C, T> x) {
    if constexpr (is_buffered<Device>::value) {
        for (index i = 0; i < x.size() && !id.eof();) {
            auto s = id.available();
            auto end = std::min(s.end(), s.begin() + (x.size() - i));
            auto ptr = s.begin();
            while (ptr != end && !x.terminator(*ptr)) x[i++] = *ptr++;
            id.move(ptr - s.begin());
            if (ptr != end) break;
        }
    } else {
        for (index i = 0; i < x.size(); ++i) {
            auto c = id.get();
            if (id.eof() || x.terminator(c)) break;
            x[i] = c;
        }
    }
    return !id.eof();
}

}  // namespace ac::io
