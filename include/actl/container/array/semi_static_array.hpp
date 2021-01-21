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
struct ssa_types {
    struct iter_types {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using reference = T;
        using difference_type = use_default;
    };

    class iterator : public iterator_facade<iterator, iter_types> {
        friend class semi_static_array<T, Is...>;
        friend struct ac::iterator_core_access;

        explicit iterator(index i, const T* arr_iter) : i_{i}, arr_iter_{arr_iter} {}

        T get() const {
            return static_array<T, Is...>{}[i_];
        }

        T dereference() const {
            return get() == dynamic_size ? *arr_iter_ : get();
        }

        void increment() {
            if (get() == dynamic_size)
                ++arr_iter_;
            ++i_;
        }

        void decrement() {
            if (get() == dynamic_size)
                --arr_iter_;
            --i_;
        }

        bool equals(const iterator& rhs) const {
            return i_ == rhs.i_;
        }

        index i_;
        const T* arr_iter_;
    };

    using size_type = index;
};

}  // namespace detail

template <class T, T... Is>
class semi_static_array
    : public range_facade<semi_static_array<T, Is...>, detail::ssa_types<T, Is...>> {
    static constexpr size_t N = (0 + ... + (Is == dynamic_size));
    using array_t = std::array<T, N>;

    array_t a_;

public:
    using iterator = typename detail::ssa_types<T, Is...>::iterator;

    explicit constexpr semi_static_array() = default;

    template <class... Ts,
              enable_int_if<((sizeof...(Ts) == N) && ... && std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(Ts&&... xs) : a_{std::forward<Ts>(xs)...} {}

    template <class R>
    explicit semi_static_array(R&& range) {
        auto iter = begin();
        for (const auto& x : range) {
            if (iter.get() == dynamic_size)
                a_[static_cast<size_t>(iter.arr_iter_ - a_.data())] = x;
            else
                ACTL_ASSERT(x == iter.get());
            ++iter;
        }
        ACTL_ASSERT(iter == end());
    }

    auto begin() const {
        return iterator{0, a_.data()};
    }
    auto end() const {
        return iterator{size(), a_.data() + a_.size()};
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
struct range_traits<semi_static_array<T, Is...>> : default_range_traits {
    static constexpr index static_size = sizeof...(Is);
};

}  // namespace ac
