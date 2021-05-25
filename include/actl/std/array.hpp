// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/range.hpp>
#include <actl/meta/rebind.hpp>
#include <array>

namespace ac {

template <class T, size_t N>
struct range_traits<std::array<T, N>> : default_range_traits
{
    static constexpr index static_size = static_cast<index>(N);
    static constexpr bool is_container = true;
};

template <class T, size_t N>
struct template_type<std::array<T, N>>
{
    using type = T;
};

template <class T, size_t N, class To>
struct rebind<std::array<T, N>, To>
{
    using type = std::array<To, N>;
};

} // namespace ac
