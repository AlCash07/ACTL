/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>
#include <actl/numeric/util/hash_access.hpp>

// Defines serialize and deserialize functions for composite types.
#define DEFINE_SERIALIZATION(...)                      \
    friend struct ac::io::serialization_access;        \
    struct io_tuple_tag;                               \
    template <class Device, class Format>              \
    index write_final(Device& od, Format& fmt) const { \
        return write(od, fmt, __VA_ARGS__);            \
    }                                                  \
    template <class Device, class Format>              \
    bool deserialize(Device& id, Format& fmt) {        \
        return read(id, fmt, __VA_ARGS__);             \
    }

// Defines hash for composite types.
#define DEFINE_HASH(...)           \
    friend struct ac::hash_access; \
    constexpr size_t hash() const { return hash_value(__VA_ARGS__); }

// Defines all the common functionality mentioned above.
#define INTROSPECT(...)               \
    DEFINE_SERIALIZATION(__VA_ARGS__) \
    DEFINE_HASH(__VA_ARGS__)
