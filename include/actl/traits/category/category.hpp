// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/traits/type_traits.hpp>

namespace ac {

struct unclassified_tag {};

template <class T, class = void>
struct category {
    using type = unclassified_tag;
};

template <class T>
struct category<T, std::void_t<typename T::category>> {
    using type = typename T::category;
};

template <class T>
using category_t = typename category<remove_cvref_t<T>>::type;

template <class Tag, class = void>
struct category_level : index_constant<0> {};

template <class T>
constexpr index category_level_v = category_level<T>::value;

template <class Tag>
struct category_level<Tag, std::void_t<typename Tag::base>>
    : index_constant<1 + category_level_v<typename Tag::base>> {};

}  // namespace ac
