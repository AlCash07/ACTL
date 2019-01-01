/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

namespace io {

template <class Code, class Device>
using deduce_code_t =
    deduce_type_t<Code, std::conditional_t<io::is_bin<Device::mode>, io::binary, io::text>>;

}  // namespace io

template <class Code, class Device, class T, class... Ts>
inline int write(Code&& code, Device& wd, const T& x, const Ts&... args) {
    return write(code, wd, x) + write(code, wd, args...);
}

template <class Code = use_default, class Device, class... Ts>
inline int write(Device& wd, const Ts&... args) {
    return write(io::deduce_code_t<Code, Device>{}, wd, args...);
}

template <class Code = use_default, class Device, class... Ts>
inline int writeln(Device& wd, const Ts&... args) {
    return write<Code>(wd, args..., '\n');
}

template <class Code, class Device, class... Ts>
inline int writeln(Code&& code, Device& wd, const Ts&... args) {
    return write(code, wd, args..., '\n');
}

template <class Code = use_default, class Device, class... Ts>
inline bool read(Device& wd, Ts&&... args) {
    return read(io::deduce_code_t<Code, Device>{}, wd, std::forward<Ts>(args)...);
}

template <class Device, class... Ts>
inline bool read(io::text, Device& wd, Ts&&... args) {
    io::itext code{wd.get()};
    bool res = read(code, wd, std::forward<Ts>(args)...);
    if (code.last != '\0') wd.unget();
    return res;
}

}  // namespace ac
