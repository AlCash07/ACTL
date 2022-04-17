// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/type_traits.hpp>

namespace ac::io {

template <
    class Device,
    class Format,
    class T,
    enable_int_if<
        std::is_empty_v<T> && !std::is_invocable_r_v<bool, T&, Device&>> = 0>
bool write_final(Device&, Format&, T&)
{
    return true;
}

template <
    class Device,
    class Format,
    class T,
    enable_int_if<
        std::is_empty_v<T> && !std::is_invocable_r_v<bool, T&, Device&>> = 0>
bool read_final(Device&, Format&, T&)
{
    return true;
}

} // namespace ac::io
