// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/device/file.hpp>

namespace ac::io {

template <class Device, bool Read = is_in<Device::mode>>
struct line_buffered_reader : Device {
    using Device::Device;
};

template <mode_t Mode, class Char>
struct line_buffered_reader<file<Mode, Char>, true>
    : line_buffered_reader<file<Mode, Char>, false> {
    using base_t = line_buffered_reader<file<Mode, Char>, false>;

    using base_t::base_t;

    size_t read(span<Char> dst) {
        if (std::fgets(dst.data(), static_cast<int>(dst.size()), this->get()))
            return std::strlen(dst.data());
        else
            return 0;
    }
};

template <class Device, bool = is_out<Device::mode>>
struct line_buffered : line_buffered_reader<Device> {
    using line_buffered_reader<Device>::line_buffered_reader;
};

template <class Device>
struct line_buffered<Device, true> : line_buffered<Device, false> {
    using line_buffered<Device, false>::line_buffered;

    size_t write(span<char_t<Device> const> src) {
        auto const* last = src.end();
        while (last != src.data() && last[-1] != '\n')
            --last;
        if (last != src.data()) {
            Device::write({src.data(), last});
            Device::flush();
        }
        Device::write({last, src.end()});
    }
};

} // namespace ac::io
