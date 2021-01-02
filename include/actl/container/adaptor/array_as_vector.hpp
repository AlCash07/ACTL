// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/std/array.hpp>
#include <actl/traits/dependent.hpp>

namespace ac {

/// Array with vector interface.
// TODO: implement full vector interface.
template <class T, size_t N, class Array = std::array<T, N>>
class array_as_vector {
public:
    using value_type = value_type_t<Array>;
    using reference = reference_t<Array>;
    using const_reference = reference_t<const Array>;
    using iterator = iterator_t<Array>;
    using const_iterator = iterator_t<const Array>;
    using size_type = size_type_t<Array>;

    array_as_vector() : end_{array_.begin()} {}

    iterator begin() noexcept {
        return array_.begin();
    }
    const_iterator begin() const noexcept {
        return array_.begin();
    }

    iterator end() noexcept {
        return end_;
    }
    const_iterator end() const noexcept {
        return end_;
    }

    size_type size() const noexcept {
        return static_cast<size_type>(end() - begin());
    }
    bool empty() const noexcept {
        return begin() == end();
    }

    reference operator[](size_type n) {
        return array_[n];
    }
    const_reference operator[](size_type n) const {
        return array_[n];
    }

    reference front() {
        return *begin();
    }
    const_reference front() const {
        return *begin();
    }

    reference back() {
        return *(end() - 1);
    }
    const_reference back() const {
        return *(end() - 1);
    }

    value_type* data() noexcept {
        return array_.data();
    }
    const value_type* data() const noexcept {
        return array_.data();
    }

    void push_back(const T& v) {
        emplace(v);
    }
    void push_back(T&& v) {
        emplace(std::move(v));
    }

    template <class... Ts>
    void emplace(Ts&&... args) {
        ACTL_ASSERT(end_ < N);
        *end_++ = T{std::forward<Ts>(args)...};
    }

    void pop_back() {
        ACTL_ASSERT(end_ != array_.begin());
        --end_;
    }

    void clear() noexcept {
        end_ = array_.begin();
    }

protected:
    Array array_;
    iterator end_;
};

}  // namespace ac
