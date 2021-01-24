// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

enum class within { inside, border, outside };

template <class Device, class Format>
index write(Device& od, Format& fmt, enum within x) {
    constexpr const char* within_name[] = {"inside", "border", "outside"};
    return write(od, fmt, within_name[static_cast<index>(x)]);
}

template <index N, class T, class U>
enum within within(const point<T, N>& lhs, const U& rhs) {
    return within(geometry_policy, lhs, rhs);
}

} // namespace ac
