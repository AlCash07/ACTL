/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/hash.hpp>
#include <actl/util/none.hpp>
#include <utility>

namespace ac {

/**
 * Container with no data that maintains size like a vector with empty elements.
 */
class dummy_container {
public:
    using size_type       = int;
    using value_type      = none;
    using reference       = none;
    using const_reference = none;
    using iterator        = int;
    using const_iterator  = int;

    explicit constexpr dummy_container(int n = 0) : n_{n} {}

    constexpr int size() const { return n_; }

    constexpr bool empty() const { return size() == 0; }

    constexpr int begin() const { return 0; }
    constexpr int end() const { return size(); }

    none operator[](int) const { return none{}; }

    template <class... Ts>
    void emplace_back(Ts...) {
        ++n_;
    }

    template <class T>
    void erase(T) { --n_; }

    void clear() { n_ = 0; }

    void resize(int n) { n_ = n; }

    void swap(dummy_container& rhs) { std::swap(n_, rhs.n_); }

protected:
    int n_;
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
