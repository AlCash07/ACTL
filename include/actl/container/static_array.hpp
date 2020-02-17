/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/compare.hpp>
#include <actl/iterator/iterator_facade.hpp>
#include <actl/std/array.hpp>

namespace ac {

/* Static array (with all the elements known at compile time) */

template <class T, T... Is>
struct static_array {
    static constexpr std::array<T, sizeof...(Is)> array = {Is...};

    explicit constexpr static_array() = default;

    template <class R>
    explicit constexpr static_array(R&& range) {
        ACTL_ASSERT(op::equal(array, range));
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

/* Partially static array where each element can have dymanic value */

template <class T, T... Is>
class semi_static_array {
    using array_t = std::array<T, (0 + ... + (Is == dynamic_size))>;

    class iterator : public iterator_facade<iterator, std::forward_iterator_tag, T, T, index> {
        friend class semi_static_array;
        friend struct iterator_core_access;

        iterator(index i, iterator_t<const array_t> ait) : i_{i}, ait_{ait} {}

        T get() const { return static_array<T, Is...>{}[i_]; }

        T dereference() const { return get() == dynamic_size ? *ait_ : get(); }

        void increment() {
            if (get() == dynamic_size) ++ait_;
            ++i_;
        }

        bool equals(const iterator& rhs) const { return i_ == rhs.i_; }

        index i_;
        iterator_t<const array_t> ait_;
    };

    array_t a_;

public:
    template <class R>
    explicit constexpr semi_static_array(R&& range) {
        auto it = begin();
        for (const auto& x : range) {
            if (it.get() == dynamic_size) {
                a_[static_cast<size_t>(it.ait_ - a_.begin())] = x;
            } else {
                ACTL_ASSERT(x == it.get());
            }
            ++it;
        }
        ACTL_ASSERT(it == end());
    }

    iterator begin() const { return iterator{0, a_.begin()}; }
    iterator end() const { return iterator{size(), a_.end()}; }

    static constexpr index size() { return index{sizeof...(Is)}; }

    constexpr T operator[](index i) const {
        auto it = begin();
        std::advance(it, i);
        return *it;
    }
};

template <class T, T... Is>
struct static_size<semi_static_array<T, Is...>> : index_constant<sizeof...(Is)> {};

}  // namespace ac
