// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant.hpp>
#include <actl/utility/use_default.hpp>
#include <limits>

namespace ac {

template<class T = size_t>
inline constexpr T dynamic_extent = static_cast<T>(-1);

template<class T, T StaticExtent>
using extent_holder_t = std::
    conditional_t<StaticExtent == dynamic_extent<T>, T, constant<StaticExtent>>;

template<class T, class Dst>
struct static_extent {
    static constexpr auto value = dynamic_extent<deduce_t<Dst, T>>;
};

template<auto N, class Dst>
struct static_extent<constant<N>, Dst> {
    static constexpr auto value = deduce_t<Dst, decltype(N)>{N};
};

template<class T, class Dst = use_default>
inline constexpr auto static_extent_v = static_extent<T, Dst>::value;

} // namespace ac
