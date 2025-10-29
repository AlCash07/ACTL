// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/associated_types.hpp>
#include <actl/std/stack.hpp>

namespace ac {

/// Queue implemented with 2 stacks : amortized O(1).
template<class T, class Stack = std::stack<T>>
class queue_using_stacks {
protected:
    Stack m_in_stack;
    Stack m_out_stack;

    void fill_out_stack() {
        if (m_out_stack.empty()) {
            while (!m_in_stack.empty()) {
                m_out_stack.push(m_in_stack.top());
                m_in_stack.pop();
            }
        }
    }

public:
    using value_type = range_value_t<Stack>;
    using reference = range_reference_t<Stack>;
    using size_type = range_size_t<Stack>;

    static_assert(std::is_same_v<T, value_type>);

    bool empty() const {
        return m_in_stack.empty() && m_out_stack.empty();
    }

    size_type size() const {
        return m_in_stack.size() + m_out_stack.size();
    }

    reference top() {
        fill_out_stack();
        return m_out_stack.top();
    }

    void push(T const& value) {
        emplace(value);
    }

    void push(T&& value) {
        emplace(std::move(value));
    }

    template<class... Ts>
    void emplace(Ts&&... args) {
        m_in_stack.push(std::forward<Ts>(args)...);
    }

    void pop() {
        fill_out_stack();
        m_out_stack.pop_back();
    }
};

} // namespace ac
