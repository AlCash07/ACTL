// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/core/manipulator.hpp>

namespace ac::io {

template<typename T>
struct size {
    T value;
};

template<typename T>
size(T&&) -> size<T>;

template<typename T>
bool write_final(Device auto& od, Format auto& fmt, size<T> size) {
    nested_scope_guard g{fmt};
    return write(od, fmt, size.value);
}

template<typename T>
bool read_final(Device auto& id, Format auto& fmt, size<T&> size) {
    nested_scope_guard g{fmt};
    return read(id, fmt, size.value);
}

} // namespace ac::io
