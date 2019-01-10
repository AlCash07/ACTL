/***************************************************************************************************
 * Macro to define read and write functions for composite types.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#define MAKE_READ_WRITE(name, ...)                         \
template <class Format, class Device>                      \
bool read_impl(Format& fmt, Device& id) {                  \
    return read(fmt, id, __VA_ARGS__);                     \
}                                                          \
template <class Format, class Device>                      \
int write_impl(Format& fmt, Device& od) const {            \
    return write(fmt, od, __VA_ARGS__);                    \
}                                                          \
template <class Format, class Device>                      \
friend bool read(Format& fmt, Device& id, name& x) {       \
    return x.read_impl(fmt, id);                           \
}                                                          \
template <class Format, class Device>                      \
friend int write(Format& fmt, Device& od, const name& x) { \
    return x.write_impl(fmt, od);                          \
}
