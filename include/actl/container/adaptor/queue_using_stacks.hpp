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
    Stack in_stack_;
    Stack out_stack_;

    void fill_out_stack() {
        if (out_stack_.empty()) {
            while (!in_stack_.empty()) {
                out_stack_.push(in_stack_.top());
                in_stack_.pop();
            }
        }
    }

public:
    using value_type = range_value_t<Stack>;
    using reference = range_reference_t<Stack>;
    using size_type = range_size_t<Stack>;

    static_assert(std::is_same_v<T, value_type>);

    bool empty() const {
        return in_stack_.empty() && out_stack_.empty();
    }

    size_type size() const {
        return in_stack_.size() + out_stack_.size();
    }

    reference top() {
        fill_out_stack();
        return out_stack_.top();
    }

    void push(T const& value) {
        emplace(value);
    }

    void push(T&& value) {
        emplace(std::move(value));
    }

    template<class... Ts>
    void emplace(Ts&&... args) {
        in_stack_.push(std::forward<Ts>(args)...);
    }

    void pop() {
        fill_out_stack();
        out_stack_.pop_back();
    }
};

} // namespace ac
