/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <array>

namespace ac {

template <class T, size_t N>
struct container_traits<std::array<T, N>> {
    using category = random_access_container_tag;

    template <class T1>
    using rebind = std::array<T1, N>;
};

}  // namespace ac

namespace std {

template <class T, size_t N>
struct hash<std::array<T, N>> {
    constexpr size_t operator()(const std::array<T, N>& arg) const { return ac::hash_range(arg); }
};

}  // namespace std
