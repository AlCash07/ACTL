/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/range.hpp>
#include <actl/util/rebind.hpp>
#include <actl/util/static_size.hpp>
#include <array>

namespace ac {

template <class T, size_t N>
struct range_traits<std::array<T, N>> : empty_range_traits {
    static constexpr bool is_container = true;
};

template <class T, size_t N>
struct static_size<std::array<T, N>> : index_constant<static_cast<index>(N)> {};

template <class T, size_t N>
struct template_type<std::array<T, N>> {
    using type = T;
};

template <class T, size_t N, class To>
struct rebind<std::array<T, N>, To> {
    using type = std::array<To, N>;
};

}  // namespace ac
