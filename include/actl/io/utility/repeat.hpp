// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/concepts.hpp>
#include <algorithm>

namespace ac::io {

template<typename Char>
struct repeat {
    explicit repeat() : c{}, count{0} {}
    explicit repeat(Char c, size_t count) : c{c}, count{count} {}

    Char c;
    size_t count;
};

template<Device Dev, typename Char>
bool write_final(Dev& od, Format auto&, repeat<Char> rep) {
    for (; 0 < rep.count; --rep.count)
        od.write(static_cast<char_t<Dev>>(rep.c));
    return true;
}

template<BufferedOutputDevice Dev, typename Char>
bool write_final(Dev& od, Format auto&, repeat<Char> rep) {
    auto s = od.output_buffer();
    if (rep.count <= s.size()) {
        std::fill_n(s.data(), rep.count, rep.c);
        od.move(static_cast<index>(rep.count));
    } else {
        std::fill_n(s.data(), s.size(), rep.c);
        od.move(static_cast<index>(s.size()));
        rep.count -= s.size();
        s = od.output_buffer();
        std::fill_n(s.data(), std::min(rep.count, s.size()), rep.c);
        // Here we assume that s references device buffer and does not
        // change.
        for (size_t n = rep.count / s.size(); n > 0; --n) {
            od.move(static_cast<index>(s.size()));
        }
        od.move(static_cast<index>(rep.count % s.size()));
    }
    return true;
}

} // namespace ac::io
