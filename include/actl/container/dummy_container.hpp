/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/numeric/hash.hpp>
#include <actl/std/utility.hpp>
#include <actl/types.hpp>
#include <actl/util/none.hpp>

namespace ac {

/**
 * Container with no data that maintains size like a vector with empty elements.
 */
class dummy_container {
public:
    using size_type       = index;
    using value_type      = none;
    using reference       = none;
    using const_reference = none;
    using iterator        = index;
    using const_iterator  = index;

    explicit constexpr dummy_container(index size = 0) : n_{size} {}

    constexpr index size() const { return n_; }

    constexpr bool empty() const { return size() == 0; }

    constexpr index begin() const { return 0; }
    constexpr index end() const { return size(); }

    none operator[](index) const { return none{}; }

    template <class... Ts>
    void emplace_back(Ts...) {
        ++n_;
    }

    template <class T>
    void erase(T) { --n_; }

    void clear() { n_ = 0; }

    void resize(index size) { n_ = size; }

    void swap(dummy_container& rhs) { std::swap(n_, rhs.n_); }

protected:
    index n_;
};

}  // namespace ac

namespace std {

template <>
struct hash<ac::dummy_container> {
    constexpr size_t operator()(const ac::dummy_container& arg) const {
        return ac::hash_value(arg.size());
    }
};

}  // namespace std
