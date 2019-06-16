/***************************************************************************************************
 * Macro to define serialize and deserialize functions for composite types.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>

#define DEFINE_SERIALIZATION(...)                    \
    friend struct ac::io::serialization_access;      \
    struct composite_io_tag;                         \
    template <class Device, class Format>            \
    index serialize(Device& od, Format& fmt) const { \
        return write(od, fmt, __VA_ARGS__);          \
    }                                                \
    template <class Device, class Format>            \
    bool deserialize(Device& id, Format& fmt) {      \
        return read(id, fmt, __VA_ARGS__);           \
    }
