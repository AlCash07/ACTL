// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

template <class D, template <class...> class B, class = void>
struct is_subcategory_of_template : std::false_type {};

template <template <class...> class B, class... Ts>
struct is_subcategory_of_template<B<Ts...>, B, void> : std::true_type {};

template <class D, template <class...> class B>
struct is_subcategory_of_template<D, B, std::void_t<typename D::base>>
    : is_subcategory_of_template<typename D::base, B> {};

template <class D, template <class...> class Base>
constexpr bool is_subcategory_of_template_v = is_subcategory_of_template<D, Base>::value;

}  // namespace ac
