// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/meta/dependent.hpp>
#include <actl/std/vector.hpp>

namespace ac {

/// Vector with queue interface.
// TODO: replace with vector_as_deque.
template <class T, class Vector = std::vector<T>>
class vector_as_queue {
public:
    using value_type = value_type_t<Vector>;
    using reference = reference_t<Vector>;
    using const_reference = reference_t<const Vector>;
    using size_type = size_type_t<Vector>;

    static_assert(std::is_same_v<T, value_type>);

    bool empty() const {
        return begin_ == vector_.size();
    }
    size_type size() const {
        return vector_.size() - begin_;
    }

    reference front() {
        return vector_[begin_];
    }
    const_reference front() const {
        return vector_[begin_];
    }

    reference back() {
        return vector_.back();
    }
    const_reference back() const {
        return vector_.back();
    }

    void push(const T& value) {
        emplace(value);
    }
    void push(T&& value) {
        emplace(std::move(value));
    }

    template <class... Ts>
    void emplace(Ts&&... args) {
        vector_.emplace_back(std::forward<Ts>(args)...);
    }

    void pop() {
        ACTL_ASSERT(begin_ < vector_.size());
        ++begin_;
    }

protected:
    Vector vector_;
    size_type begin_ = 0;  // not an iterator since vector can reallocate
};

}  // namespace ac
