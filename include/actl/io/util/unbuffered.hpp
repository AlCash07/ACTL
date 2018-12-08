/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>

namespace ac::io {

template <class Device, bool = is_in<Device::mode>>
class unbuffered_reader : public Device {
public:
    static_assert(!is_line_buffered<Device::mode>);

    using Device::Device;
};

template <class Device>
class unbuffered_reader<Device, true> : public unbuffered_reader<Device, false> {
public:
    using unbuffered_reader<Device, false>::unbuffered_reader;

    template <class Predicate>
    int read_until(char* dst, int count, Predicate is_delimiter) {
        int n = count;
        while (count > 0 && !Device::eof()) {
            char c = Device::get();
            if (is_delimiter(c)) break;
            --count;
            *dst++ = c;
        }
        return n - count;
    }
};

template <class Device, bool = is_out<Device::mode>>
class unbuffered : public unbuffered_reader<Device> {
public:
    using unbuffered_reader<Device>::unbuffered_reader;
};

template <class Device>
class unbuffered<Device, true> : public unbuffered<Device, false> {
public:
    using unbuffered<Device, false>::unbuffered;

    void write_fill(char c, int count) {
        for (; count > 0; --count) Device::put(c);
    }
};

}  // namespace ac::io
