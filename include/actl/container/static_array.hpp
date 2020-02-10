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

/* Static array (with all the elements known at compile time) */

template <class T, T...>
struct static_array {
    explicit constexpr static_array() = default;

    template <class A>
    explicit constexpr static_array(A&& array) {
        ACTL_ASSERT(std::empty(array));
    }

    static constexpr index size() { return 0; }

    T operator[](index) const {
        ACTL_ASSERT(false);
        return {};
    }
};

template <class T, T I0, T... Is>
struct static_array<T, I0, Is...> {
    explicit constexpr static_array() = default;

    template <class A>
    explicit constexpr static_array(A&& array) {
        ACTL_ASSERT(std::size(array) == size());
        for (index i = 0; i < size(); ++i) {
            ACTL_ASSERT(array[i] == (*this)[i]);
        }
    }

    static constexpr index size() { return 1 + sizeof...(Is); }

    constexpr T operator[](index i) const {
        return i == 0 ? I0 : static_array<T, Is...>{}[i - 1];
    }
};

template <class T, T... Is>
struct static_size<static_array<T, Is...>> : index_constant<sizeof...(Is)> {};

/* Partially static array */

template <class T, size_t D, T... Is>
class semi_static_array {
public:
    template <class A>
    explicit constexpr semi_static_array(A&& array) {
        ACTL_ASSERT(std::size(array) == size());
        size_t ai = 0;
        for (index i = 0; i < size(); ++i) {
            T value = static_array<T, Is...>{}[i];
            if (value == dynamic_size) {
                a_[ai++] = array[i];
            } else {
                ACTL_ASSERT(array[i] == value);
            }
        }
    }

    static constexpr index size() { return sizeof...(Is); }

    constexpr T operator[](index i) const {
        if (T result = static_array<T, Is...>{}[i]; result != dynamic_size) return result;
        size_t ai = 0;
        while (i > 0) ai += static_array<T, Is...>{}[--i] == dynamic_size;
        return a_[ai];
    }

private:
    std::array<T, D> a_;
};

template <class T, size_t D, T... Is>
struct static_size<semi_static_array<T, D, Is...>> : index_constant<sizeof...(Is)> {};

template <class T, size_t D, T... Is>
struct part_static_array_impl {
    using type = semi_static_array<T, D, Is...>;
};

template <class T, T... Is>
struct part_static_array_impl<T, 0, Is...> {
    using type = static_array<T, Is...>;
};

template <class T, T I0, T... Is>
struct part_static_array_impl<T, 1 + sizeof...(Is), I0, Is...> {
    using type = std::array<T, 1 + sizeof...(Is)>;
};

template <class T, T... Is>
using part_static_array =
    typename part_static_array_impl<T, (0 + ... + (Is == dynamic_size)), Is...>::type;

}  // namespace ac
