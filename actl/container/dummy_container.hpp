/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

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

    explicit dummy_container(int n = 0) : n_{n} {}

    int size() const { return n_; }

    bool empty() const { return size() == 0; }

    none operator[](int) const { return none{}; }

    template <class... Ts>
    void emplace(Ts...) { ++n_; }

    template <class T>
    void erase(T) { --n_; }

    void clear() { n_ = 0; }

    void resize(int n) { n_ = n; }

    void swap(dummy_container& other) { std::swap(n_, other.n_); }

protected:
    int n_;
};

}  // namespace ac
