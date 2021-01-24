// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template <class D, class B, class = void>
struct is_subcategory_of : std::false_type {};

template <class B>
struct is_subcategory_of<B, B, void> : std::true_type {};

template <class D, class B>
struct is_subcategory_of<D, B, std::void_t<typename D::base>>
    : is_subcategory_of<typename D::base, B> {};

template <class D, class Base>
inline constexpr bool is_subcategory_of_v = is_subcategory_of<D, Base>::value;

} // namespace ac
