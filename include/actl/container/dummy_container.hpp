// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/integer_iterator.hpp>
#include <actl/numeric/utility/hash_access.hpp>
#include <actl/range/traits.hpp>
#include <actl/std/utility.hpp>
#include <actl/types.hpp>
#include <actl/utility/none.hpp>

namespace ac {

/// Container with no data that maintains size like a vector with empty elements.
class dummy_container {
public:
    using size_type = index;
    using value_type = none;
    using reference = none;
    using const_reference = none;
    using iterator = integer_iterator<index>;
    using const_iterator = integer_iterator<index>;

    constexpr dummy_container() = default;
    explicit constexpr dummy_container(index size) : n_{size} {}

    constexpr index size() const {
        return n_;
    }

    constexpr bool empty() const {
        return size() == 0;
    }

    constexpr auto begin() const {
        return iterator{0};
    }
    constexpr auto end() const {
        return iterator{size()};
    }

    none operator[](index) const {
        return none{};
    }

    template <class... Ts>
    void emplace_back(Ts...) {
        ++n_;
    }

    template <class T>
    void erase(T) {
        --n_;
    }

    void resize(index size) {
        n_ = size;
    }

    void swap(dummy_container& rhs) {
        std::swap(n_, rhs.n_);
    }

private:
    friend struct ac::hash_access;

    size_t hash() const {
        return hash_value(size());
    }

    index n_ = 0;
};

template <>
struct range_traits<dummy_container> : default_range_traits {
    static constexpr bool is_container = true;
};

}  // namespace ac
