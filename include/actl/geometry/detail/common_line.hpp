/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/detail/line_test.hpp>
#include <actl/std/utility.hpp>
#include <functional>

namespace ac::detail {

template <template <class> class Less, class T>
inline auto max_endpoint(const std::pair<T, uint8_t>& lhs, const std::pair<T, uint8_t>& rhs) {
    if (lhs.second == endpoint::free) {
        if (rhs.second != endpoint::free) {
            return rhs;
        } else {
            return Less<T>{}(lhs.first, rhs.first) ? lhs : rhs;
        }
    } else {
        if (rhs.second == endpoint::free) {
            return lhs;
        } else {
            return Less<std::pair<T, uint8_t>>{}(lhs, rhs) ? rhs : lhs;
        }
    }
}

template <class R, index N, class T0, class K0, class T1, class K1>
inline std::pair<bool, any_line<R, N>> common_line(const line<T0, N, K0>& lhs,
                                                   const line<T1, N, K1>& rhs) {
    using point_t = point<R, N>;
    auto la = std::pair{(point_t)lhs.start, lhs.start_kind()};
    auto lb = std::pair{(point_t)lhs.end(), lhs.end_kind()};
    if (lhs.vector < point<T0, N>{}) std::swap(la, lb);
    auto ra = std::pair{(point_t)rhs.start, rhs.start_kind()};
    auto rb = std::pair{(point_t)rhs.end(), rhs.end_kind()};
    if (rhs.vector < point<T1, N>{}) std::swap(ra, rb);
    la = max_endpoint<std::less>(la, ra);
    lb = max_endpoint<std::greater>(lb, rb);
    auto dst = make_any_line(la.first, la.second, lb.first, lb.second);
    return std::pair{endpoint_test(dst.start_kind(), lb.first, la.first), dst};
}

}  // namespace ac::detail
