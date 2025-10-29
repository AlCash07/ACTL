// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/std/array.hpp>

namespace ac {

/// Array with vector interface.
// TODO: implement full vector interface.
template<class T, size_t N, class Array = std::array<T, N>>
class array_as_vector {
public:
    using value_type = range_value_t<Array>;
    using reference = range_reference_t<Array>;
    using const_reference = range_reference_t<Array const>;
    using iterator = range_iterator_t<Array>;
    using const_iterator = range_iterator_t<Array const>;
    using size_type = range_size_t<Array>;

    array_as_vector() : m_end{m_array.begin()} {}

    iterator begin() noexcept {
        return m_array.begin();
    }

    const_iterator begin() const noexcept {
        return m_array.begin();
    }

    iterator end() noexcept {
        return m_end;
    }

    const_iterator end() const noexcept {
        return m_end;
    }

    size_type size() const noexcept {
        return static_cast<size_type>(end() - begin());
    }

    bool empty() const noexcept {
        return begin() == end();
    }

    reference operator[](size_type n) {
        return m_array[n];
    }

    const_reference operator[](size_type n) const {
        return m_array[n];
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
        return m_array.data();
    }

    value_type const* data() const noexcept {
        return m_array.data();
    }

    void push_back(T const& v) {
        emplace(v);
    }

    void push_back(T&& v) {
        emplace(std::move(v));
    }

    template<class... Ts>
    void emplace(Ts&&... args) {
        AC_ASSERT(m_end < N);
        *m_end++ = T{std::forward<Ts>(args)...};
    }

    void pop_back() {
        AC_ASSERT(m_end != m_array.begin());
        --m_end;
    }

    void clear() noexcept {
        m_end = m_array.begin();
    }

protected:
    Array m_array;
    iterator m_end;
};

} // namespace ac
