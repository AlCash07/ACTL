/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <array>
#include <vector>

namespace ac {

/**
 * Array with vector interface.
 */
template <class T, size_t N, class Array = std::array<T, N>>
class array_as_vector : public Array {
public:
    // TODO: implement full vector interface.
    using value_type      = typename Array::value_type;
    using reference       = typename Array::reference;
    using const_reference = typename Array::const_reference;
    using iterator        = typename Array::iterator;
    using const_iterator  = typename Array::const_iterator;
    using size_type       = typename Array::size_type;

    iterator       begin() noexcept { return array_.begin(); }
    const_iterator begin() const noexcept { return array_.begin(); }
    iterator       end() noexcept { return end_; }
    const_iterator end() const noexcept { return end_; }

    array_as_vector() : end_{array_.begin()} {}

    size_type size() const noexcept { return static_cast<size_type>(end() - begin()); }
    bool      empty() const noexcept { return begin() == end(); }

    reference       operator[](size_type n) { return array_[n]; }
    const_reference operator[](size_type n) const { return array_[n]; }

    reference       front() { return *begin(); }
    const_reference front() const { return *begin(); }

    reference       back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }

    value_type*       data() noexcept { return array_.data(); }
    const value_type* data() const noexcept { return array_.data(); }

    void push_back(const T& v) { emplace(v); }
    void push_back(T&& v) { emplace(std::move(v)); }

    template <class... Ts>
    void emplace(Ts&&... args) {
        ACTL_ASSERT(end_ < N);
        *end_++ = T{std::forward<Ts>(args)...};
    }

    void pop_back() {
        ACTL_ASSERT(end_ != array_.begin());
        --end_;
    }

    void clear() noexcept { end_ = array_.begin(); }

protected:
    Array    array_;
    iterator end_;
};

}  // namespace ac
