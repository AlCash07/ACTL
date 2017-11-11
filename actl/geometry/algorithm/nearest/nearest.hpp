#pragma once

#include <actl/geometry/traits/id.hpp>
#include <actl/geometry/traits/policy.hpp>
#include <utility>

namespace ac {

template <class Policy, class T0, class T1, class = geometry::enable_if_swap_t<T0, T1>>
inline auto nearest(const Policy& policy, const T0& lhs, const T1& rhs) {
    auto pair = nearest(policy, rhs, lhs);
    return std::make_pair(pair.second, pair.first);
}

template <class T, class... Ts, class = geometry::disable_if_policy_t<T>>
inline auto nearest(const T& arg, const Ts&... args) {
    return nearest(use_default(), arg, args...);
}

}  // namespace ac
