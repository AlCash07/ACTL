/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <utility>

namespace ac::io {

template <class T1, class T2>
struct is_composite<std::pair<T1, T2>> : std::true_type {};

template <class Device, class Format, class T1, class T2>
inline index serialize(Device& od, Format& fmt, const std::pair<T1, T2>& x) {
    return write(od, fmt, x.first, x.second);
}

template <class Device, class Format, class T1, class T2>
inline bool deserialize(Device& id, Format& fmt, std::pair<T1, T2>& x) {
    // const_cast is used to be able to read std::map<Key, Value>::value_type which is
    // std::pair<const Key, Value>.
    return read(id, fmt, const_cast<std::remove_const_t<T1>&>(x.first), x.second);
}

}  // namespace ac::io
