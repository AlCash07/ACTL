// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/static_array.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/range/facade/range_facade.hpp>

namespace ac {

template <class T, T... Is>
class semi_static_array;

namespace detail {

template <class T, T... Is>
class ssa_iterator : public iterator_facade<ssa_iterator<T, Is...>,
                                            iterator_types<std::bidirectional_iterator_tag, T, T>> {
    friend class semi_static_array<T, Is...>;
    friend struct ac::iterator_core_access;

    explicit ssa_iterator(index i, const T* ait) : i_{i}, ait_{ait} {}

    T get() const {
        return static_array<T, Is...>{}[i_];
    }

    T dereference() const {
        return get() == dynamic_size ? *ait_ : get();
    }

    void increment() {
        if (get() == dynamic_size)
            ++ait_;
        ++i_;
    }

    void decrement() {
        if (get() == dynamic_size)
            --ait_;
        --i_;
    }

    bool equals(const ssa_iterator& rhs) const {
        return i_ == rhs.i_;
    }

    index i_;
    const T* ait_;
};

}  // namespace detail

template <class T, T... Is>
class semi_static_array : public range_facade<semi_static_array<T, Is...>,
                                              range_types<detail::ssa_iterator<T, Is...>, index>> {
    static constexpr size_t N = (0 + ... + (Is == dynamic_size));
    using array_t = std::array<T, N>;

    array_t a_;

public:
    using iterator = detail::ssa_iterator<T, Is...>;

    explicit constexpr semi_static_array() = default;

    template <class... Ts,
              enable_int_if<((sizeof...(Ts) == N) && ... && std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(Ts&&... xs) : a_{std::forward<Ts>(xs)...} {}

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

    auto begin() const {
        return iterator{0, a_.begin()};
    }
    auto end() const {
        return iterator{size(), a_.end()};
    }

    static constexpr index size() {
        return index{sizeof...(Is)};
    }

    T operator[](index i) const {
        auto it = begin();
        std::advance(it, i);
        return *it;
    }

    friend void swap(semi_static_array& lhs, semi_static_array& rhs) {
        lhs.a_.swap(rhs.a_);
    }
};

template <class T, T... Is>
struct static_size<semi_static_array<T, Is...>> : index_constant<sizeof...(Is)> {};

}  // namespace ac
