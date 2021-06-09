// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/io.hpp>
#include <algorithm>

namespace ac::io {

template <class Char>
struct repeat
{
    explicit repeat() : c{}, count{0} {}
    explicit repeat(Char c, index count) : c{c}, count{count} {}

    Char c;
    index count;
};

template <class Device, class Format, class Char>
bool write_final(Device& od, Format&, repeat<Char> x)
{
    if constexpr (has_output_buffer<Device>::value)
    {
        auto s = od.output_data();
        if (x.count <= s.size())
        {
            std::fill_n(s.data(), x.count, x.c);
            od.move(x.count);
        }
        else
        {
            std::fill_n(s.data(), s.size(), x.c);
            od.move(s.size());
            x.count -= s.size();
            s = od.output_data();
            std::fill_n(s.data(), std::min(x.count, s.size()), x.c);
            // Here we assume that s references device buffer and does not
            // change.
            for (index n = x.count / s.size(); n > 0; --n)
            {
                od.move(s.size());
            }
            od.move(x.count % s.size());
        }
    }
    else
    {
        for (; 0 < x.count; --x.count)
            od.write(static_cast<char_t<Device>>(x.c));
    }
    return true;
}

} // namespace ac::io
