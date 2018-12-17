/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>

namespace ac {

template <class T, class P = use_default>
class angle_compare {
public:
    explicit constexpr angle_compare(const point<T>& origin = {})
        : origin_(origin), zero_(!origin) {}

    template <class T0, class T1>
    bool operator()(const point<T0>& lhs, const point<T1>& rhs) const {
        comparable_area_points<P> policy;
        return (zero_ ? area(policy, lhs, rhs) : area(policy, lhs, rhs, origin_)) > 0;
    }

private:
    const point<T> origin_;
    const bool     zero_;
};

template <class P = use_default, class T>
inline auto make_angle_compare(const point<T>& origin) {
    return angle_compare<T, P>(origin);
}

}  // namespace ac
