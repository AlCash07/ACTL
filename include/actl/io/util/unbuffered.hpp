/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/manip/till.hpp>

namespace ac::io {

template <class Device, bool = is_in<Device::mode>>
class unbuffered_reader : public Device {
public:
    static_assert(!is_line_buffered<Device::mode>);

    using Device::Device;
};

template <class Device>
class unbuffered_reader<Device, true> : public unbuffered_reader<Device, false> {
protected:
    using base_t = unbuffered_reader<Device, false>;
    using Char = char_t<Device>;

public:
    using base_t::base_t;
    using base_t::read;

    template <class T>
    index read(till<Char, T> dst) {
        Char* dstPtr = dst.data();
        index count = dst.size();
        for (; 0 < count; --count) {
            Char c = Device::get();
            if (Device::eof() || dst.terminator(c)) break;
            *dstPtr++ = c;
        }
        return dst.size() - count;
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

    void write_fill(char_t<Device> c, index count) {
        for (; 0 < count; --count) Device::put(c);
    }
};

}  // namespace ac::io
