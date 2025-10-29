// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/utility/index.hpp>

namespace ac::io {

template<typename T>
struct raw {
    T value;
};

template<typename T>
raw(T) -> raw<T>;

template<typename T>
bool write_final(Device auto& od, Format auto& fmt, raw<T> const& x) {
    return write_final(od, fmt, x.value);
}

} // namespace ac::io
