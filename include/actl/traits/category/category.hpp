// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/traits/type_traits.hpp>

namespace ac {

struct unclassified_tag {};

template <class T, class>
struct category_sfinae {
    using type = unclassified_tag;
};

template <class T>
struct category_sfinae<T, void_t<typename T::category>> {
    using type = typename T::category;
};

template <class T>
struct category : category_sfinae<T, void> {};

template <class T>
using category_t = typename category<T>::type;

}  // namespace ac
