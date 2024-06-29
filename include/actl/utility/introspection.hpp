// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/serialization_access.hpp>
#include <actl/numeric/utility/hash_access.hpp>

// Defines write_final and read_final functions for composite types.
#define DEFINE_SERIALIZATION(...)                               \
    friend struct ac::io::serialization_access;                 \
    struct is_io_tuple;                                         \
    bool write_final(Device auto& od, Format auto& fmt) const { \
        return write(od, fmt, __VA_ARGS__);                     \
    }                                                           \
    bool read_final(Device auto& id, Format auto& fmt) {        \
        return read(id, fmt, __VA_ARGS__);                      \
    }

// Defines hash for composite types.
#define DEFINE_HASH(...)                \
    friend struct ac::hash_access;      \
    constexpr size_t hash() const {     \
        return hash_value(__VA_ARGS__); \
    }

// Defines all the common functionality mentioned above.
#define INTROSPECT(...)               \
    DEFINE_SERIALIZATION(__VA_ARGS__) \
    DEFINE_HASH(__VA_ARGS__)
