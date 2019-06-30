/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits/geometry_traits.hpp>
#include <actl/util/square_root.hpp>
#include <actl/util/use_default.hpp>

namespace ac::geometry {

template <class P, class... Ts>
using product_t = deduce_t<P, scalar_t<Ts...>>;

template <class S, class... Ts>
using sqrt_t = deduce_t<S, decltype(math::sqrt(std::declval<scalar_t<Ts...>>()))>;

template <class R, class... Ts>
using ratio_t = deduce_t<R, sqrt_t<use_default, Ts...>>;

template <class P, class... Ts>
using comparable_sqrt_t = square_root<product_t<P, Ts...>>;

}  // namespace ac::geometry
