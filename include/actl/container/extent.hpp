// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/use_default.hpp>
#include <actl/meta/constant.hpp>
#include <limits>

namespace ac {

template<typename T = size_t>
inline constexpr T dynamic_extent = static_cast<T>(-1);

template<typename T, T StaticExtent>
using extent_holder_t = std::
    conditional_t<StaticExtent == dynamic_extent<T>, T, constant<StaticExtent>>;

template<typename T, typename Target>
struct static_extent {
    static constexpr auto value = dynamic_extent<deduce_t<Target, T>>;
};

template<auto N, typename Target>
struct static_extent<constant<N>, Target> {
    static constexpr auto value = deduce_t<Target, decltype(N)>{N};
};

template<typename T, typename Target = use_default>
inline constexpr auto static_extent_v = static_extent<T, Target>::value;

} // namespace ac
