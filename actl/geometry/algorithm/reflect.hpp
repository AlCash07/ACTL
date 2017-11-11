#pragma once

#include <actl/geometry/algorithm/project/project.hpp>

namespace ac {

template <class Policy, int N, class T0, class T1>
inline auto reflect(const Policy& policy, const point<N, T0>& src, const T1& dst) {
    return 2 * project(policy, src, dst) - src;
}

template <int N, class T0, class T1>
inline auto reflect(const point<N, T0>& src, const T1& dst) {
    return reflect(use_default(), src, dst);
}

}  // namespace ac
