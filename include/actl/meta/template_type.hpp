// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_at.hpp>

namespace ac {

template <class T>
struct template_type
{
    using type = T;
};

template <class T, size_t N>
struct template_type<T[N]>
{
    using type = T;
};

template <template <class...> class C, class... Ts>
struct template_type<C<Ts...>>
{
    using type = type_at_t<0, Ts...>;
};

} // namespace ac
