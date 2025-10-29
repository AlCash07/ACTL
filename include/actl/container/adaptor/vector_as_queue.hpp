// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/std/vector.hpp>

namespace ac {

/// Vector with queue interface.
// TODO: replace with vector_as_deque.
template<typename T, typename Vector = std::vector<T>>
class vector_as_queue {
public:
    using value_type = range_value_t<Vector>;
    using reference = range_reference_t<Vector>;
    using const_reference = range_reference_t<Vector const>;
    using size_type = range_size_t<Vector>;

    static_assert(std::is_same_v<T, value_type>);

    bool empty() const {
        return m_begin == m_vector.size();
    }

    size_type size() const {
        return m_vector.size() - m_begin;
    }

    reference front() {
        return m_vector[m_begin];
    }

    const_reference front() const {
        return m_vector[m_begin];
    }

    reference back() {
        return m_vector.back();
    }

    const_reference back() const {
        return m_vector.back();
    }

    void push(T const& value) {
        emplace(value);
    }

    void push(T&& value) {
        emplace(std::move(value));
    }

    template<typename... Ts>
    void emplace(Ts&&... args) {
        m_vector.emplace_back(std::forward<Ts>(args)...);
    }

    void pop() {
        AC_ASSERT(m_begin < m_vector.size());
        ++m_begin;
    }

protected:
    Vector m_vector;
    size_type m_begin = 0; // not an iterator since vector can reallocate
};

} // namespace ac
