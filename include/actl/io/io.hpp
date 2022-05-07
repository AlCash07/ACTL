// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/byte.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/serialization_access.hpp>
#include <actl/io/device/mode.hpp>
#include <actl/io/device/traits.hpp>
#include <actl/io/format/binary/binary.hpp>
#include <actl/io/format/composed_format.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::io {

/* Device */

template <mode_t Mode, class Char>
struct device : device_base
{
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    using char_type = Char;

    static constexpr mode_t mode = Mode;
};

/* Read and write. Absence of std::forward is intentional here to convert rvalue
   references into
   lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
bool write(Device&& od, Format&& fmt, Ts const&... args)
{
    if constexpr (is_format_v<Format>)
        return (... && detail::write_impl(od, fmt, fmt, args));
    else
        return write(od, deduce_format(od), fmt, args...);
}

template <class Device, class Format, class... Ts>
bool read(Device&& id, Format&& fmt, Ts&&... args)
{
    if constexpr (is_format_v<Format>)
        return (... && detail::read_impl(id, fmt, fmt, args));
    else
        return read(id, deduce_format(id), fmt, args...);
}

} // namespace ac::io
