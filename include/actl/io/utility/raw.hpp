// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/utility/index.hpp>

namespace ac::io {

template <class T>
struct raw
{
    T value;
};

template <class T>
raw(T) -> raw<T>;

template <class Device, class Format, class T>
bool write_final(Device& od, Format& fmt, const raw<T>& x)
{
    return write_final(od, fmt, x.value);
}

} // namespace ac::io
