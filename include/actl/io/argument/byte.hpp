// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/device/traits.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::io {

template <class T>
using enable_int_if_byte =
    enable_int_if<std::is_arithmetic_v<T> && sizeof(T) == 1>;

template <class Device, class Format, class B, enable_int_if_byte<B> = 0>
bool write_final(Device& od, Format&, B byte)
{
    return od.write(static_cast<char_t<Device>>(byte)) == 1;
}

template <class Device, class Format, class B, enable_int_if_byte<B> = 0>
bool read_final(Device& id, Format&, B& byte)
{
    byte = static_cast<B>(id.get());
    return !id.eof();
}

} // namespace ac::io
