/***************************************************************************************************
 * read and write support for standard complex types:
 * - pair
 * - complex
 * - vector (size and then the elements)
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/read.hpp>
#include <actl/io/write.hpp>
#include <complex>
#include <utility>
#include <vector>

namespace ac {

template <class Device, class T1, class T2>
inline bool read(Device& in, std::pair<T1, T2>& arg) { return read(in, arg.first, arg.second); }

template <class Device, class T>
inline bool read(Device& in, std::complex<T>& arg) {
    T real, imag;
    if (!read(in, real, imag)) return false;
    arg.real(real);
    arg.imag(imag);
    return true;
}

template <class Device, class T>
inline bool read(Device& in, std::vector<T>& arg) {
    uint32_t size;
    if (!read(in, size)) return false;
    arg.resize(size);
    return read(in, arg.begin(), arg.end());
}

template <class Device, class T1, class T2>
inline int write(Device& out, const std::pair<T1, T2>& arg) {
    return write(out, arg.first, arg.second);
}

template <class Device, class T>
inline int write(Device& out, const std::complex<T>& arg) {
    return write(out, real(arg), imag(arg));
}

template <class Device, class T>
inline int write(Device& out, const std::vector<T>& arg) {
    return write(out, static_cast<uint32_t>(arg.size()), arg.begin(), arg.end());
}

}  // namespace ac
