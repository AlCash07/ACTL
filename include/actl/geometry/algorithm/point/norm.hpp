/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>
#include <actl/util/square_root.hpp>

namespace ac {

template <class P = use_default>
struct comparable_norm : geometry::policy {};

template <class P = use_default, class S = use_default>
struct standard_norm : geometry::policy {};

template <class P, index N, class T>
inline auto norm(comparable_norm<P>, const point<T, N>& point) {
    return deferred_sqrt(abs<P>(point));
}

template <class P, class S, index N, class T>
inline auto norm(standard_norm<P, S>, const point<T, N>& point) {
    return (geometry::sqrt_t<S, T>)norm(comparable_norm<P>{}, point);
}

template <index N, class T>
inline auto norm(const point<T, N>& point) {
    return norm(standard_norm<>{}, point);
}

}  // namespace ac
