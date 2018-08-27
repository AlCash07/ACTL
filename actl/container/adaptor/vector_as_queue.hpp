/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <vector>

namespace ac {

/**
 * Vector with queue interface.
 */
template <class T, class Vector = std::vector<T>>
class vector_as_queue {
public:
    // TODO: replace with vector_as_deque.
    using value_type      = typename Vector::value_type;
    using reference       = typename Vector::reference;
    using const_reference = typename Vector::const_reference;
    using size_type       = typename Vector::size_type;
    static_assert(std::is_same_v<T, value_type>, "");

    bool empty() const { return vector_.empty(); }

    size_type size() const { return vector_.size(); }

    reference       top() { return vector_[begin_]; }
    const_reference top() const { return vector_[begin_]; }

    void push(const T& value) { emplace(value); }
    void push(T&& value) { emplace(std::move(value)); }

    template <class... Ts>
    void emplace(Ts&&... args) {
        vector_.emplace_back(std::forward<Ts>(args)...);
    }

    void pop() {
        ACTL_ASSERT(begin_ < vector_.size());
        ++begin_;
    }

protected:
    Vector    vector_;
    size_type begin_ = 0;  // not an iterator since vector can reallocate
};

}  // namespace ac
