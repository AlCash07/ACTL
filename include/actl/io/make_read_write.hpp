/***************************************************************************************************
 * Macro to define read and write functions for composite types.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#define MAKE_READ_WRITE(name, ...) \
template <class Code, class Device> \
bool read_impl(Device& dr) { return read<Code>(dr, __VA_ARGS__); } \
template <class Code, class Device> \
int write_impl(Device& dw) const { return write<Code>(dw, __VA_ARGS__); } \
template <class Code, class Device> \
friend bool read(Device& dr, name& arg) { return arg.read_impl<Code>(dr); } \
template <class Code, class Device> \
friend int write(Device& dw, const name& arg) { return arg.write_impl<Code>(dw); }
