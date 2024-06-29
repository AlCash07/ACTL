// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/concepts.hpp>
#include <algorithm>

namespace ac::io {

template<class Char>
struct repeat {
    explicit repeat() : c{}, count{0} {}
    explicit repeat(Char c, size_t count) : c{c}, count{count} {}

    Char c;
    size_t count;
};

template<Device Dev, class Char>
bool write_final(Dev& od, Format auto&, repeat<Char> x) {
    for (; 0 < x.count; --x.count)
        od.write(static_cast<char_t<Dev>>(x.c));
    return true;
}

template<BufferedOutputDevice Dev, class Char>
bool write_final(Dev& od, Format auto&, repeat<Char> x) {
    auto s = od.output_buffer();
    if (x.count <= s.size()) {
        std::fill_n(s.data(), x.count, x.c);
        od.move(static_cast<index>(x.count));
    } else {
        std::fill_n(s.data(), s.size(), x.c);
        od.move(static_cast<index>(s.size()));
        x.count -= s.size();
        s = od.output_buffer();
        std::fill_n(s.data(), std::min(x.count, s.size()), x.c);
        // Here we assume that s references device buffer and does not
        // change.
        for (size_t n = x.count / s.size(); n > 0; --n) {
            od.move(static_cast<index>(s.size()));
        }
        od.move(static_cast<index>(x.count % s.size()));
    }
    return true;
}

} // namespace ac::io
