// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/byte.hpp>
#include <actl/io/concepts.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/serialization_access.hpp>
#include <actl/io/device/mode.hpp>
#include <actl/io/format/binary/binary.hpp>
#include <actl/io/format/composed_format.hpp>

namespace ac::io {

/* Device */

template<mode_t Mode, typename Char>
struct device : device_base {
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    using char_type = Char;

    static constexpr mode_t mode = Mode;
};

/* Read and write. Absence of std::forward is intentional here to convert rvalue
   references into
   lvalue references, because I/O doesn't operate with rvalues. */

template<typename... Ts>
bool write(Device auto&& od, Format auto&& fmt, Ts const&... args) {
    return (... && detail::write_impl(od, fmt, fmt, args));
}

template<typename... Ts>
bool write(Device auto&& od, Ts const&... args) {
    return write(od, deduce_format(od), args...);
}

template<typename... Ts>
bool read(Device auto&& id, Format auto&& fmt, Ts&&... args) {
    return (... && detail::read_impl(id, fmt, fmt, args));
}

template<typename... Ts>
bool read(Device auto&& id, Ts&&... args) {
    return read(id, deduce_format(id), args...);
}

} // namespace ac::io
