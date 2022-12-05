// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/traits/associated_types.hpp>
#include <actl/utility/none.hpp>

namespace ac::graph {

template <class T>
struct list_value
{
    using type = value_t<T>;
};

template <>
struct list_value<none>
{
    using type = none;
};

template <class T>
using list_value_t = typename list_value<T>::type;

} // namespace ac::graph
