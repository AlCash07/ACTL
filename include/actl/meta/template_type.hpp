// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>

namespace ac {

template<class T>
struct template_type {
    using type = T;
};

template<class T, size_t N>
struct template_type<T[N]> {
    using type = T;
};

template<template<class...> class C, class T0, class... Ts>
struct template_type<C<T0, Ts...>> {
    using type = T0;
};

} // namespace ac
