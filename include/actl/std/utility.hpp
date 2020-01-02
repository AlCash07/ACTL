/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/compare.hpp>
#include <actl/io/io.hpp>
#include <actl/numeric/hash.hpp>
#include <utility>

namespace ac {

template <class T1, class T2>
inline constexpr size_t hash_value(const std::pair<T1, T2>& x) {
    return hash_value(x.first, x.second);
}

namespace op {

template <class Policy, class T1, class T2>
inline bool perform(Equal, const Policy& policy, const std::pair<T1, T2>& lhs,
                    const std::pair<T1, T2>& rhs) {
    return equal(policy, lhs.first, rhs.first) && equal(policy, lhs.second, rhs.second);
}

template <class Policy, class T1, class T2>
inline bool perform(Less, const Policy& policy, const std::pair<T1, T2>& lhs,
                    const std::pair<T1, T2>& rhs) {
    int v = sgn(policy, lhs.first, rhs.first);
    return v < 0 || (v == 0 && less(policy, lhs.second, rhs.second));
}

}  // namespace op

namespace io {

template <class T1, class T2>
struct is_tuple<std::pair<T1, T2>> : std::true_type {};

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

}  // namespace io

}  // namespace ac
