/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/contiguous_range_facade.hpp>
#include <actl/std/array.hpp>

namespace ac {

template <class T, T... Is>
class static_array
    : public contiguous_range_facade<static_array<T, Is...>, range_types<const T*, index>> {
    static constexpr std::array<T, sizeof...(Is)> array = {Is...};

public:
    explicit constexpr static_array() = default;

    template <class R>
    explicit constexpr static_array(R&& range) {
        ACTL_ASSERT(math::equal(array, range));
    }

    static constexpr const T* data() { return array.data(); }

    static constexpr index size() { return index{array.size()}; }
};

template <class T, T... Is>
struct static_size<static_array<T, Is...>> : index_constant<sizeof...(Is)> {};

}  // namespace ac
