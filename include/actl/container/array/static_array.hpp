// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/facade/contiguous_range_facade.hpp>
#include <actl/std/array.hpp>

namespace ac {

namespace detail {

template <class T>
struct sa_types
{
    using iterator = const T*;
    using size_type = index;
};

} // namespace detail

template <class T, T... Is>
class static_array
    : public contiguous_range_facade<
          static_array<T, Is...>,
          detail::sa_types<T>>
{
    static constexpr std::array<T, sizeof...(Is)> array = {Is...};

public:
    explicit constexpr static_array() = default;

    template <class R>
    explicit constexpr static_array(R&& range)
    {
        ACTL_ASSERT(equal(array, range));
    }

    static constexpr const T* data()
    {
        return array.data();
    }

    static constexpr index size()
    {
        return index{array.size()};
    }
};

template <class T, T... Is>
struct range_traits<static_array<T, Is...>> : default_range_traits
{
    static constexpr index static_size = sizeof...(Is);
};

} // namespace ac
