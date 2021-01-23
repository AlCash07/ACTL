// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/category.hpp>

namespace ac {

// clang-format off
struct scalar_tag            { using base = unclassified_tag; };
struct arithmetic_tag        { using base = scalar_tag; };
struct floating_point_tag    { using base = arithmetic_tag; };
struct integral_tag          { using base = arithmetic_tag; };
struct signed_integral_tag   { using base = integral_tag; };
struct unsigned_integral_tag { using base = integral_tag; };
struct boolean_tag           { using base = unsigned_integral_tag; };
// clang-format on

template <class T>
struct category_sfinae<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
    static auto f() {
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_signed_v<T>)
                return signed_integral_tag{};
            else
                return unsigned_integral_tag{};
        } else {
            if constexpr (std::is_floating_point_v<T>)
                return floating_point_tag{};
            else
                return arithmetic_tag{};
        }
    }

    using type = decltype(f());
};

template <>
struct category<bool> {
    using type = boolean_tag;
};

template <class T, T X>
struct category<std::integral_constant<T, X>> : category<T> {};

template <class T>
struct category_sfinae<T*, std::enable_if_t<!std::is_object_v<T>>> {
    using type = scalar_tag;
};

}  // namespace ac
