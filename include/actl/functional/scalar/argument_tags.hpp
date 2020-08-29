/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation/argument_traits.hpp>

namespace ac::math {

struct arithmetic_tag : scalar_tag     { using base = scalar_tag; };
struct float_tag      : arithmetic_tag { using base = arithmetic_tag; };
struct integral_tag   : arithmetic_tag { using base = arithmetic_tag; };
struct boolean_tag    : integral_tag   { using base = integral_tag; };

template <class T>
struct category_impl<T, std::enable_if_t<std::is_arithmetic_v<T> && !std::is_integral_v<T> &&
                                         !std::is_floating_point_v<T>>> {
    using type = arithmetic_tag;
};

template <class T>
struct category_impl<T, std::enable_if_t<std::is_floating_point_v<T>>> {
    using type = float_tag;
};

template <class T>
struct category_impl<T, std::enable_if_t<std::is_integral_v<T>>> {
    using type = integral_tag;
};

template <>
struct category_impl<bool> {
    using type = boolean_tag;
};

}  // namespace ac::math
