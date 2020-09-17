/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/std/array.hpp>

namespace ac {

template <class T, T... Is>
struct static_array {
    static constexpr std::array<T, sizeof...(Is)> array = {Is...};

    explicit constexpr static_array() = default;

    template <class R>
    explicit constexpr static_array(R&& range) {
        ACTL_ASSERT(math::equal(array, range));
    }

    static constexpr auto begin() { return array.begin(); }
    static constexpr auto end() { return array.end(); }

    static constexpr const T* data() { return array.data(); }

    static constexpr index size() { return index{array.size()}; }

    constexpr T operator[](index i) const {
        ACTL_ASSERT(0 <= i && i < size());
        return array[(size_t)i];
    }
};

template <class T, T... Is>
struct static_size<static_array<T, Is...>> : index_constant<sizeof...(Is)> {};

}  // namespace ac
