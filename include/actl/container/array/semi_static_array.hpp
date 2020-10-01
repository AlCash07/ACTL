/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/static_array.hpp>
#include <actl/iterator/iterator_facade.hpp>

namespace ac {

template <class T, T... Is>
class semi_static_array {
    using array_t = std::array<T, (0 + ... + (Is == dynamic_size))>;

    class iterator
        : public iterator_facade<iterator, iterator_types<std::forward_iterator_tag, T, T, index>> {
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
    explicit semi_static_array(R&& range) {
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

    T operator[](index i) const {
        auto it = begin();
        std::advance(it, i);
        return *it;
    }
};

template <class T, T... Is>
struct static_size<semi_static_array<T, Is...>> : index_constant<sizeof...(Is)> {};

}  // namespace ac
