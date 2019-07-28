/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>

namespace ac {

// TODO: make this a policy instead of a functor.
template <class Policy, class Origin = void>
struct angle_compare {
    Policy policy;
    Origin origin;

    template <class T0, class T1>
    bool operator()(const point<T0>& lhs, const point<T1>& rhs) const {
        return less(policy, 0, area(policy, lhs - origin, rhs - origin));
    }
};

template <class Policy>
struct angle_compare<Policy, void> {
    Policy policy;

    template <class T0, class T1>
    bool operator()(const point<T0>& lhs, const point<T1>& rhs) const {
        return less(policy, 0, area(policy, lhs, rhs));
    }
};

template <class... Ts>
angle_compare(Ts&&...) -> angle_compare<Ts...>;

}  // namespace ac
