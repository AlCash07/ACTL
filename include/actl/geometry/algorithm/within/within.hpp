// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

enum class within { inside, border, outside };

bool write(Device auto& od, Format auto& fmt, enum within x) {
    constexpr char const* within_name[] = {"inside", "border", "outside"};
    return write(od, fmt, within_name[static_cast<index>(x)]);
}

template<index N, typename TL, typename R>
enum within within(point<TL, N> const& l, R const& r) {
    return within(geometry_policy, l, r);
}

} // namespace ac
