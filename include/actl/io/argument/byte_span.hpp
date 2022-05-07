// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/argument/byte.hpp>
#include <actl/range/span.hpp>

namespace ac::io {

template <
    class Device,
    class Format,
    class B,
    size_t N,
    enable_int_if_byte<B> = 0>
bool write_final(Device& od, Format&, span<B, N> s)
{
    return od.write(
               {reinterpret_cast<char_t<Device> const*>(s.data()), s.size()}) ==
           s.size();
}

template <
    class Device,
    class Format,
    class B,
    size_t N,
    enable_int_if_byte<B> = 0>
bool read_final(Device& id, Format&, span<B, N>& s)
{
    return id.read({reinterpret_cast<char_t<Device>*>(s.data()), s.size()}) ==
           s.size();
}

} // namespace ac::io
