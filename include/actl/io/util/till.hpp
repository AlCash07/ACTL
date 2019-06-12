/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

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

template <class Device, class Format, class P>
inline bool deserialize(Device& id, Format&, till<span<char_t<Device>>, P> x) {
    if constexpr (has_input_buffer<Device>::value) {
        for (index i = 0; i < x.value.size();) {
            auto s = id.input_data();
            auto end = std::min(s.end(), s.begin() + (x.value.size() - i));
            auto ptr = s.begin();
            while (ptr != end && !x.terminator(*ptr)) x.value[i++] = *ptr++;
            id.move(ptr - s.begin());
            if (s.empty() || ptr != end) break;
        }
    } else {
        for (auto& c : x.value) {
            auto t = id.get();
            if (id.eof() || x.terminator(t)) break;
            c = t;
        }
    }
    return true;
}

}  // namespace ac::io
